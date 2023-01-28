#include <algorithm>  // std::copy, std::copy_if, std::count_if, std::find, std::for_each, std::max_element, std::sort, std::transform, std::unique
#include <cctype>     // std::alpha, std::isdigit, std::isspace
#include <cstdio>     // std::FILE, std::fclose, std::fopen
#include <cstdlib>    // std::system
#include <ctype.h>    // std::tolower
#include <deque>      // std::deque
#include <filesystem> // std::filesystem::current_path
#include <ios>        // std::streamsize
#include <iostream>   // std::cerr, std::cin, std::cout, std::endl
#include <iterator>   // std::back_inserter, std::ostream_iterator
#include <limits>     // std::numeric_limits
#include <map>        // std::map
#include <sstream>    // std::istringstream, std::ostringstream
#include <string>     // std::string
#include <tuple>      // std::get, std::tuple 
#include <utility>    // std::make_pair, std::pair
#include <vector>     // std::vector

#include <graph/AndOrGraph.hpp>
#include <graph/DiGraph.hpp>

#include <main/Assembly.hpp>
#include <main/Component.hpp>

#include <pomdp/Action.hpp>
#include <pomdp/Observation.hpp>
#include <pomdp/Pomdp.hpp>
#include <pomdp/PomdpxWriter.hpp>

#include <tinyxml2.h>

#include <utils/utils.hpp> // utils::cast_cstring, utils::split_cstring

using Subassembly = std::vector<Component>;
using State = std::vector<Subassembly>;
using Intention = std::vector<State>;

Pomdp::Pomdp(const std::string &description)
    : description{description}, file_name{}, assembly{}, state_graph{}, intention_graph{},
      import_action_ids{false}, intention_ids{}, action_ids{}, observation_ids{}, action_obs_mapping{},
      num_intentions{}, num_actions{}, num_observations{},
      init_belief{}, state_trans_probabilities{}, observation_probabilities{}, rewards{}, discount{},
      robot_actions{}, pomdpx_file_path{}, policy_file_path{}, policy{}
{
    std::transform(this->description.begin(), this->description.end(), std::back_inserter(this->file_name),
                   [](char c) {
                       if (std::isalpha(c))
                           return static_cast<char>(std::tolower(c));
                       else if (std::isdigit(c))
                           return c;
                       else if (std::isspace(c))
                           return '_';
                       else
                           return '\0';
                   });
}

Pomdp::Pomdp(const std::string &description, const Assembly &assembly, bool import_action_ids)
    : description{description}, file_name{}, assembly{assembly}, state_graph{}, intention_graph{},
      import_action_ids{import_action_ids}, intention_ids{}, action_ids{}, observation_ids{}, action_obs_mapping{},
      num_intentions{}, num_actions{}, num_observations{},
      init_belief{}, state_trans_probabilities{}, observation_probabilities{}, rewards{}, discount{},
      robot_actions{}, pomdpx_file_path{}, policy_file_path{}, policy{}
{
    std::transform(this->description.begin(), this->description.end(), std::back_inserter(this->file_name),
                   [](char c) {
                       if (std::isalpha(c))
                           return static_cast<char>(std::tolower(c));
                       else if (std::isdigit(c))
                           return c;
                       else if (std::isspace(c))
                           return '_';
                       else
                           return '\0';
                   });
    // ACTIONS
    AndOrGraph<Subassembly> ao_graph{this->assembly.get_ao_graph()};
    std::vector<std::tuple<Subassembly, std::vector<Subassembly>, int>> and_edges{ao_graph.get_edges()};
    for (const auto &edge : and_edges) 
    {
        Action action{std::get<1>(edge), std::get<0>(edge)};
        if (this->import_action_ids)
            this->_add_action(action, std::get<2>(edge));
        else
            this->_add_action(action);
    }
    this->_add_action(Action{}); // wait action

    // robot can only wait or extend existing subassemblies (i.e. grasp one part + tool)
    this->robot_actions.push_back(Action{}); // wait action
    std::vector<Action> actions{this->get_actions()};
    std::copy_if(actions.begin(), actions.end(), std::back_inserter(this->robot_actions),
                 [](const Action &action) {
                     std::vector<Subassembly> preconditions{action.get_preconditions()};
                     int count = std::count_if(preconditions.begin(), preconditions.end(),
                                               [](const Subassembly &subasm) {
                                                   return (subasm.size() == 1);
                                               });
                     return (count == 1);
                 });

    // ASSEMBLY STATES
    this->state_graph = this->_generate_state_graph();
    this->state_graph.set_name(this->file_name + "_state_graph");

    // INTENTIONS
    this->intention_graph = this->_generate_intention_graph();
    this->intention_graph.set_name(this->file_name + "_intention_graph");
    
    std::vector<Intention> ig_nodes{this->intention_graph.get_nodes()};
    std::for_each(ig_nodes.begin(), ig_nodes.end(), [this](const auto &node) {
        this->_add_intention(node);
    });

    // OBSERVATIONS
    this->_add_observation(Observation{}); // wait observation

    for (const Action &action : this->get_actions())
    {
        std::vector<Component> manip_components{};
        for (const Subassembly &subasm : action.get_preconditions())
        {
            if (subasm.size() == 1)
                manip_components.push_back(subasm.front());
        }
        std::sort(manip_components.begin(), manip_components.end());

        Observation observation{manip_components, true};
        this->_add_observation(observation);

        int action_id{};
        this->_get_id(action, action_id);

        int observation_id{};
        this->_get_id(observation, observation_id);

        if (action == Action{}) // in case of wait action
        {
            int wait_action_id{};
            this->_get_id(Action{}, wait_action_id);

            int wait_observation_id{};
            this->_get_id(Observation{}, wait_observation_id);

            this->action_obs_mapping.emplace(std::make_pair(wait_action_id, wait_observation_id));
        }
        else
            this->action_obs_mapping.emplace(std::make_pair(action_id, observation_id));
    }

    this->num_intentions = this->get_intentions().size();
    this->num_actions = this->get_actions().size();
    this->num_observations = this->get_observations().size();

    this->_init_belief();
    this->_init_state_trans();
    this->_init_observation_func();
    this->_init_reward_func();
    this->discount = 0.9;
}

bool Pomdp::_get_id(const Intention &intention, int &out) const
{
    auto it = std::find_if(this->intention_ids.begin(), this->intention_ids.end(),
                           [&intention](const auto &m) {
                               return (m.second == intention);
                           });

    if (it != intention_ids.end())
    {
        out = it->first;
        return true;
    }
    else
    {
        out = this->intention_ids.size();
        return false;
    }
}

bool Pomdp::_get_id(const Action &action, int &out) const
{
    auto it = std::find_if(this->action_ids.begin(), this->action_ids.end(),
                           [&action](const auto &m) {
                               return (m.second == action);
                           });

    if (it != action_ids.end())
    {
        out = it->first;
        return true;
    }
    else
    {
        out = this->action_ids.size();
        return false;
    }
}

bool Pomdp::_get_id(const Observation &observation, int &out) const
{
    auto it = std::find_if(this->observation_ids.begin(), this->observation_ids.end(),
                           [&observation](const auto &m) {
                               return (m.second == observation);
                           });

    if (it != observation_ids.end())
    {
        out = it->first;
        return true;
    }
    else
    {
        out = this->observation_ids.size();
        return false;
    }
}

bool Pomdp::_exist_file(const std::string &file_path) const
{
    if (std::FILE *file = std::fopen(file_path.c_str(), "r"))
    {
        std::fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

void Pomdp::_add_intention(const Intention &intention)
{
    int intention_id{};
    if (!this->_get_id(intention, intention_id))
        this->intention_ids.emplace(std::make_pair(intention_id, intention));
}

void Pomdp::_add_action(const Action &action)
{
    int action_id{};
    if (!this->_get_id(action, action_id))
        this->action_ids.emplace(std::make_pair(action_id, action));
}

void Pomdp::_add_action(const Action &action, int id)
{
    int action_id{};
    if (!this->_get_id(action, action_id))
        this->action_ids.emplace(std::make_pair(id, action));
}

void Pomdp::_add_observation(const Observation &observation)
{
    int observation_id{};
    if (!this->_get_id(observation, observation_id))
        this->observation_ids.emplace(std::make_pair(observation_id, observation));
}

void Pomdp::_init_belief()
{
    this->init_belief = std::vector<double>(this->num_intentions, 0.0);
    std::vector<Intention> start_intentions{this->intention_graph.get_root_nodes()};

    for (const Intention &intention : this->get_intentions())
    {
        int intention_id{};
        if (this->_get_id(intention, intention_id))
        {
            if (std::find(start_intentions.begin(), start_intentions.end(), intention) != start_intentions.end())
                this->init_belief.at(intention_id) = 1.0 / start_intentions.size();
            else
                this->init_belief.at(intention_id) = 0.0;
        }
    }
}

void Pomdp::_init_state_trans()
{
    this->state_trans_probabilities =
        std::vector<std::vector<std::vector<double>>>(this->num_intentions, std::vector<std::vector<double>>(this->num_actions, std::vector<double>(this->num_intentions, 0.0)));

    for (int current_intention_id{0}; current_intention_id < this->num_intentions; ++current_intention_id)
    {
        Intention current_intention{this->intention_ids.at(current_intention_id)};
        for (int action_id{0}; action_id < this->num_actions; ++action_id)
        {
            Action action{this->action_ids.at(action_id)};
            std::vector<Intention> next_intentions{this->_get_state_trans(current_intention, action)};

            double uniform_trans_prob{1.0 / next_intentions.size()};
            std::for_each(next_intentions.begin(), next_intentions.end(),
                          [this, &current_intention_id, &action_id, &uniform_trans_prob](const Intention next_intention) {
                              int next_intention_id{};
                              this->_get_id(next_intention, next_intention_id);
                              this->state_trans_probabilities.at(current_intention_id).at(action_id).at(next_intention_id) = uniform_trans_prob;
                          });
        }
    }
}

void Pomdp::_init_observation_func()
{
    this->observation_probabilities =
        std::vector<std::vector<std::vector<double>>>(this->num_intentions, std::vector<std::vector<double>>(this->num_actions, std::vector<double>(this->num_observations, 0.0)));

    for (int intention_id{0}; intention_id < this->num_intentions; ++intention_id)
    {
        Intention intention{this->intention_ids.at(intention_id)};
        for (int action_id{0}; action_id < this->num_actions; ++action_id)
        {
            std::vector<Observation> observations{this->_get_observations(intention)};

            double x{1.0 / (observations.size() - 1.0 + 1.0 / 4.0)}; // x * (#observations - 1) + x / 4 = 1;
            std::for_each(observations.begin(), observations.end(),
                          [this, &intention_id, &action_id, &x](const Observation observation) {
                              int observation_id{};
                              this->_get_id(observation, observation_id);
                              if (observation == Observation{})
                                  this->observation_probabilities.at(intention_id).at(action_id).at(observation_id) = x / 4.0;
                              else
                                  this->observation_probabilities.at(intention_id).at(action_id).at(observation_id) = x;
                          });
        }
    }
}

void Pomdp::_init_reward_func()
{
    this->rewards =
        std::vector<std::vector<double>>(this->num_intentions, std::vector<double>(this->num_actions, 0.0));

    int ACT_ACC_INTENTION_TASK_ALLOC_REWARD = 10;
    int ACT_NOT_ACC_INTENTION_REWARD = -50;
    int WAIT_REWARD = 0;
    int ACT_NOT_ACC_TASK_ALLOC_REWARD = -2;

    for (int intention_id{0}; intention_id < this->num_intentions; ++intention_id)
    {
        Intention intention{this->intention_ids.at(intention_id)};
        for (int action_id{0}; action_id < this->num_actions; ++action_id)
        {
            Action action{this->action_ids.at(action_id)};

            std::vector<Action> possible_actions{this->_get_actions(intention)};
            if (std::find(possible_actions.begin(), possible_actions.end(), action) != possible_actions.end())
            {
                if (action == Action{})
                    this->rewards.at(intention_id).at(action_id) = WAIT_REWARD;
                else
                {
                    if (std::find(this->robot_actions.begin(), this->robot_actions.end(), action) != this->robot_actions.end())
                        this->rewards.at(intention_id).at(action_id) = ACT_ACC_INTENTION_TASK_ALLOC_REWARD;
                    else
                        this->rewards.at(intention_id).at(action_id) = ACT_NOT_ACC_TASK_ALLOC_REWARD;
                }
            }
            else
                this->rewards.at(intention_id).at(action_id) = ACT_NOT_ACC_INTENTION_REWARD;
        }
    }
}

DiGraph<State, Action> Pomdp::_generate_state_graph() const
{
    DiGraph<State, Action> state_graph{};
    std::deque<State> open_states{};

    std::vector<Subassembly> root_subasms{this->assembly.get_ao_graph().get_root_nodes()};
    std::transform(root_subasms.begin(), root_subasms.end(), std::back_inserter(open_states),
                   [](const Subassembly &root_subasm) { return State{root_subasm}; });

    while (!open_states.empty())
    {
        State open_state{open_states.front()};
        open_states.pop_front();

        for (const Subassembly &subasm : open_state)
        {
            for (const std::tuple<Subassembly, std::vector<Subassembly>, int> &edge : this->assembly.get_ao_graph().get_edges(subasm))
            {
                State successor_state{open_state};
                successor_state.erase(std::find(successor_state.begin(), successor_state.end(), subasm));

                std::vector<Subassembly> successor_subasms = std::get<1>(edge);
                successor_state.insert(successor_state.end(), successor_subasms.begin(), successor_subasms.end());
                std::sort(successor_state.begin(), successor_state.end());

                Action action{successor_subasms, subasm};
                int action_id{};
                if (this->import_action_ids)
                    action = this->action_ids.at(std::get<2>(edge));
                else if (this->_get_id(action, action_id))
                    action = this->action_ids.at(action_id);
                
                state_graph.add_edge(successor_state, open_state, action);
                open_states.push_back(successor_state);
            }
        }
    }
    return state_graph;
}

DiGraph<Intention, Action> Pomdp::_generate_intention_graph() const
{
    DiGraph<State, Action> state_graph{this->state_graph.reverse()};

    DiGraph<Intention, Action> intention_graph{};
    std::deque<Intention> open_intentions{};

    std::vector<State> root_states{state_graph.get_root_nodes()};
    std::transform(root_states.begin(), root_states.end(), std::back_inserter(open_intentions),
                   [](const State &root_state) { return Intention{root_state}; });

    while (!open_intentions.empty())
    {
        Intention open_intention{open_intentions.front()};
        open_intentions.pop_front();

        State open_state{open_intention.back()};
        for (const State &successor_state : state_graph.get_successors(open_state))
        {
            Intention successor_intention{open_intention};
            successor_intention.push_back(successor_state);

            Action action{state_graph.get_edge_attr(std::make_pair(open_state, successor_state))};
            intention_graph.add_edge(successor_intention, open_intention, action);
            open_intentions.push_back(successor_intention);
        }
    }
    return intention_graph;
}

std::vector<Intention> Pomdp::_get_state_trans(const Intention &current_intention, const Action &action) const
{
    std::vector<Intention> next_intentions{};

    int action_id{};
    if (this->_get_id(action, action_id))
    {
        std::vector<Intention> interm_intentions{};
        if (action == Action{}) // in case robot performs wait action
            interm_intentions.push_back(current_intention);
        else
        {
            std::vector<Intention> successors{this->intention_graph.get_successors(current_intention)};
            std::copy_if(successors.begin(), successors.end(), std::back_inserter(interm_intentions),
                         [this, &action, &current_intention](const Intention &successor) {
                             return (this->intention_graph.get_edge_attr(std::make_pair(current_intention, successor)) == action);
                         });
        }

        for (const Intention interm_intention : interm_intentions)
        {
            std::vector<Intention> successors{this->intention_graph.get_successors(interm_intention)};
            next_intentions.insert(next_intentions.end(), successors.begin(), successors.end());
            next_intentions.push_back(interm_intention); // in case human decides to wait
        }
    }

    // remove duplicates
    std::sort(next_intentions.begin(), next_intentions.end());
    next_intentions.erase(std::unique(next_intentions.begin(), next_intentions.end()), next_intentions.end());

    if (next_intentions.empty()) // in case no successor states exist
        next_intentions.push_back(current_intention);

    return next_intentions;
}

std::vector<Action> Pomdp::_get_actions(const Intention &intention) const
{
    std::vector<Action> actions{Action{}}; // wait action is always possible

    for (const Intention &successor : this->intention_graph.get_successors(intention))
    {
        Action action{this->intention_graph.get_edge_attr(std::make_pair(intention, successor))};
        actions.push_back(action);
    }

    return actions;
}

std::vector<Action> Pomdp::_get_prev_actions(const Intention &intention) const
{
    std::vector<Action> prev_actions{Action{}}; // wait action is always possible

    for (const Intention &predecessor : this->intention_graph.get_predecessors(intention))
    {
        Action action{this->intention_graph.get_edge_attr(std::make_pair(predecessor, intention))};
        prev_actions.push_back(action);
    }

    return prev_actions;
}

std::vector<Observation> Pomdp::_get_observations(const Intention &intention) const
{
    std::vector<Observation> observations{};

    for (const Action &prev_action : this->_get_prev_actions(intention))
    {
        int prev_action_id{};
        this->_get_id(prev_action, prev_action_id);
        int observation_id{this->action_obs_mapping.at(prev_action_id)};
        observations.push_back(this->observation_ids.at(observation_id));
    }

    return observations;
}

std::string Pomdp::get_description() const
{
    return this->description;
}

std::vector<Intention> Pomdp::get_intentions() const
{
    std::vector<Intention> intentions{};
    std::transform(this->intention_ids.begin(), this->intention_ids.end(), std::back_inserter(intentions),
                   [](const auto &intention_id) { return intention_id.second; });
    return intentions;
}

std::vector<Action> Pomdp::get_actions() const
{
    std::vector<Action> actions{};
    std::transform(this->action_ids.begin(), this->action_ids.end(), std::back_inserter(actions),
                   [](const auto &action_id) { return action_id.second; });
    return actions;
}

std::vector<Observation> Pomdp::get_observations() const
{
    std::vector<Observation> observations{};
    std::transform(this->observation_ids.begin(), this->observation_ids.end(), std::back_inserter(observations),
                   [](const auto &observation_id) { return observation_id.second; });
    return observations;
}

DiGraph<State, Action> Pomdp::get_state_graph() const
{
    return this->state_graph;
}

DiGraph<Intention, Action> Pomdp::get_intention_graph() const
{
    return this->intention_graph;
}

std::vector<double> Pomdp::get_init_belief() const
{
    return this->init_belief;
}

std::vector<std::vector<std::vector<double>>> Pomdp::get_state_trans_probabilities() const
{
    return this->state_trans_probabilities;
}

std::vector<std::vector<std::vector<double>>> Pomdp::get_observation_probabilities() const
{
    return this->observation_probabilities;
}

std::vector<std::vector<double>> Pomdp::get_rewards() const
{
    return this->rewards;
}

double Pomdp::get_discount() const
{
    return this->discount;
}

int Pomdp::get_num_states() const
{
    return this->num_intentions;
}

int Pomdp::get_num_actions() const
{
    return this->num_actions;
}

int Pomdp::get_num_observations() const
{
    return this->num_observations;
}

void Pomdp::convert_to_pomdpx(const std::string &output_loc)
{
    std::ostringstream ss_file_path{};
    ss_file_path << output_loc << '/'
                 << this->file_name << ".pomdpx";

    bool convert{true};
    if (this->_exist_file(ss_file_path.str()))
    {
        char input{};
        std::cout << "[Convert Pomdpx]: Pomdpx file already exist in: " << ss_file_path.str() << '\n';
        do
        {
            std::cout << "\t > Do you want to convert anyway? [Y/N]: ";
            std::cin >> input;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard the input buffer
            std::cout << std::endl;

            input = std::tolower(input);
        } while (input != 'y' && input != 'n');
        convert = (input == 'y' ? true : false);
    }

    if (convert)
    {
        PomdpxWriter pomdpx{*this};
        pomdpx.write(ss_file_path.str());
        this->pomdpx_file_path = ss_file_path.str();
    }
}

void Pomdp::solve()
{
    std::istringstream ss_current_path{std::filesystem::current_path()};
    std::vector<std::string> segment_list{utils::split_string<std::string>(ss_current_path.str(), '/')};
    segment_list.pop_back(); // skip last 'build' prefix

    std::ostringstream ss_output_path{};
    ss_output_path << utils::vector_to_string<std::string>(segment_list, "/")
                   << "/output/" << this->file_name << ".policy";

    std::ostringstream ss_solver_path{};
    ss_solver_path << utils::vector_to_string<std::string>(segment_list, "/")
                   << '/' << "extern/sarsop/src/pomdpsol";

    bool solve{true};
    if (this->_exist_file(ss_output_path.str()))
    {
        char input{};
        std::cout << "[Solve Policy]: Policy file already exist in: " << ss_output_path.str() << '\n';
        do
        {
            std::cout << "\t> Do you want to solve anyway? [Y/N]: ";
            std::cin >> input;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard the input buffer
            std::cout << std::endl;

            input = std::tolower(input);
        } while (input != 'y' && input != 'n');
        solve = (input == 'y' ? true : false);
    }

    if (solve)
    {
        std::ostringstream solver_cmd{};
        solver_cmd << ss_solver_path.str() << " " << this->pomdpx_file_path
                   << " --output " << ss_output_path.str();

        std::system(solver_cmd.str().c_str());
        this->policy_file_path = ss_output_path.str();
    }
}

void Pomdp::convert_and_solve(const std::string &output_loc)
{
    this->convert_to_pomdpx(output_loc);
    this->solve();
}

void Pomdp::import_pomdpx(const std::string &file_path)
{
    if (this->_exist_file(file_path))
        this->pomdpx_file_path = file_path;
    else
        std::cerr << "[Import Pomdpx]: Pomdpx file doesn't exist: " << file_path
                  << std::endl;
}

void Pomdp::import_policy(const std::string &file_path)
{
    if (this->_exist_file(file_path))
    {
        tinyxml2::XMLDocument xml_doc{};
        if (xml_doc.LoadFile(file_path.c_str()) == tinyxml2::XML_SUCCESS)
        {
            tinyxml2::XMLConstHandle doc_handle(&xml_doc);
            const tinyxml2::XMLElement *alpha_vec_ptr{doc_handle.FirstChildElement("Policy").FirstChildElement("AlphaVector").ToElement()};

            if (alpha_vec_ptr)
            {
                std::cout << "[Import Policy]: Importing " << alpha_vec_ptr->Attribute("numVectors") << " alpha vectors."
                          << std::endl;

                for (const tinyxml2::XMLNode *vector_node{alpha_vec_ptr->FirstChildElement()}; vector_node; vector_node = vector_node->NextSibling())
                {
                    const tinyxml2::XMLElement *vector_element = vector_node->ToElement();
                    int action_id{utils::cast_cstring<int>(vector_element->Attribute("action"))};
                    std::vector<double> alpha_values{utils::split_cstring<double>(vector_element->GetText())};

                    auto it = this->policy.find(action_id);
                    if (it == this->policy.end())
                        this->policy.emplace(std::make_pair(action_id, std::vector<std::vector<double>>{alpha_values}));
                    else
                        it->second.push_back(alpha_values);
                }

                this->policy_file_path = file_path;
            }
            else
                std::cerr << "[Import Policy]: File is probably empty: " << file_path
                          << std::endl;
        }
        else
            std::cerr << "[Import Policy]: TinyXML2 cannot load policy file: " << file_path
                      << std::endl;
    }
    else
        std::cerr << "[Import Policy]: Policy file doesn't exist: " << file_path
                  << std::endl;
}

Action Pomdp::get_optimal_action(const std::vector<double> &belief) const
{
    Action optimal_action{};

    std::map<int, double> action_values{};
    if (!this->policy.empty())
    {
        for (const std::pair<int, std::vector<std::vector<double>>> &action_alpha_vec_pair : this->policy)
        {
            std::vector<double> vector_values{};
            std::transform(action_alpha_vec_pair.second.begin(), action_alpha_vec_pair.second.end(),
                           std::back_inserter(vector_values), [&belief](const std::vector<double> &alpha_vec) {
                               return utils::dot_product<double>(alpha_vec, belief);
                           });

            double max_value{*std::max_element(vector_values.begin(), vector_values.end())};
            action_values.emplace(std::make_pair(action_alpha_vec_pair.first, max_value));
        }

        auto it = std::max_element(action_values.begin(), action_values.end(),
                                   [](const std::pair<int, double> &p1, const std::pair<int, double> &p2) {
                                       return p1.second < p2.second;
                                   });
        optimal_action = this->action_ids.at(it->first);
    }
    else
        std::cerr << "[Action selection]: No policy available!"
                  << std::endl;

    return optimal_action;
}