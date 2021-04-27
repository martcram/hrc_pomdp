#include <algorithm> // std::copy, std::find, std::remove_if, std::sort
#include <cctype>    // std::isdigit, std::isspace
#include <cstdlib>   // std::system
#include <iostream>  // std::cout
#include <fstream>   // std::ifstream
#include <sstream>   // std::ostringstream
#include <stdexcept>
#include <stdio.h>
#include <string>        // std:getline, std::string
#include <unordered_map> // std::unordered_map
#include <utility>       // std::make_pair, std::pair
#include <vector>        // std::vector

#include <graph/DiGraph.hpp>
#include <graph/Graph.hpp>

#include <main/Assembly.hpp>
#include <main/Component.hpp>

#include <pomdp/Action.hpp>
#include <pomdp/BayesFilter.hpp>
#include <pomdp/Observation.hpp>
#include <pomdp/Pomdp.hpp>

#include <utils/utils.hpp>

using Subassembly = std::vector<Component>;

std::string exec(const char *cmd)
{
    char buffer[128];
    std::string result = "";
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            result += buffer;
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

std::unordered_map<int, int> init_action_ids(const std::vector<Action> &actions)
{
    std::unordered_map<int, int> action_id_mapping{};

    Action action{std::vector<Subassembly>{}, Subassembly{}}; // wait action
    auto it = std::find(actions.begin(), actions.end(), action);
    action_id_mapping.emplace(std::make_pair(std::distance(actions.begin(), it), 0));

    std::ifstream input_file{"/home/student/catkin_ws/src/hrc_demonstrator/src/action_mapping.txt"};
    while (!input_file.eof())
    {
        std::string s_action_id{};
        std::getline(input_file, s_action_id);

        int action_id{};
        std::stringstream ss_action_id{s_action_id};
        ss_action_id >> action_id;

        std::string line{};
        std::getline(input_file, line);
        std::vector<std::string> action_segments{utils::split_string<std::string>(line, '>')};

        std::vector<std::string> s_preconditions{utils::split_string<std::string>(action_segments.at(0), '|')};

        std::vector<Subassembly> preconditions{};
        for (std::string &s_subasm : s_preconditions)
        {
            std::vector<std::string> s_components{utils::split_string<std::string>(s_subasm, ',')};

            Subassembly subasm{};
            for (std::string &s_component : s_components)
            {
                s_component.erase(std::remove_if(s_component.begin(), s_component.end(), isspace), s_component.end());

                Component component{s_component};
                subasm.push_back(component);
            }
            std::sort(subasm.begin(), subasm.end());
            preconditions.push_back(subasm);
        }
        std::sort(preconditions.begin(), preconditions.end());

        std::string s_effect{action_segments.at(1)};
        std::vector<std::string> s_components{utils::split_string<std::string>(s_effect, ',')};

        Subassembly effect{};
        for (std::string &s_component : s_components)
        {
            s_component.erase(std::remove_if(s_component.begin(), s_component.end(), isspace), s_component.end());

            Component component{s_component};
            effect.push_back(component);
        }
        std::sort(effect.begin(), effect.end());

        std::string temp{};
        std::getline(input_file, temp); // ignore blank line

        Action action{preconditions, effect};
        auto it = std::find(actions.begin(), actions.end(), action);
        action_id_mapping.emplace(std::make_pair(std::distance(actions.begin(), it), action_id));
    }

    return action_id_mapping;
}

int main()
{
    std::vector<std::pair<Component, Component>> liaison_edges{
        {Component{"Inkt"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Punt"}},
        {Component{"Punt"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Dop"}},
        {Component{"Schacht"}, Component{"Stop"}}};

    Graph<Component> connection_graph{liaison_edges};

    std::vector<std::pair<Component, Component>> obstruction_edges_posX{
        {Component{"Dop"}, Component{"Inkt"}},
        {Component{"Dop"}, Component{"Cartridge"}},
        {Component{"Dop"}, Component{"Punt"}},
        {Component{"Dop"}, Component{"Schacht"}},
        {Component{"Dop"}, Component{"Stop"}},
        {Component{"Punt"}, Component{"Cartridge"}},
        {Component{"Punt"}, Component{"Stop"}},
        {Component{"Punt"}, Component{"Schacht"}},
        {Component{"Punt"}, Component{"Inkt"}},
        {Component{"Cartridge"}, Component{"Stop"}},
        {Component{"Schacht"}, Component{"Stop"}},
        {Component{"Inkt"}, Component{"Stop"}}};

    std::vector<std::pair<Component, Component>> obstruction_edges_posY{
        {Component{"Punt"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Punt"}},
        {Component{"Punt"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Punt"}},
        {Component{"Punt"}, Component{"Dop"}},
        {Component{"Dop"}, Component{"Punt"}},
        {Component{"Cartridge"}, Component{"Inkt"}},
        {Component{"Inkt"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Dop"}},
        {Component{"Dop"}, Component{"Cartridge"}},
        {Component{"Inkt"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Inkt"}},
        {Component{"Inkt"}, Component{"Dop"}},
        {Component{"Dop"}, Component{"Inkt"}},
        {Component{"Dop"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Dop"}},
        {Component{"Stop"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Stop"}}};

    std::vector<std::pair<Component, Component>> obstruction_edges_posZ{
        {Component{"Punt"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Punt"}},
        {Component{"Punt"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Punt"}},
        {Component{"Punt"}, Component{"Dop"}},
        {Component{"Dop"}, Component{"Punt"}},
        {Component{"Cartridge"}, Component{"Inkt"}},
        {Component{"Inkt"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Dop"}},
        {Component{"Dop"}, Component{"Cartridge"}},
        {Component{"Inkt"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Inkt"}},
        {Component{"Inkt"}, Component{"Dop"}},
        {Component{"Dop"}, Component{"Inkt"}},
        {Component{"Dop"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Dop"}},
        {Component{"Stop"}, Component{"Schacht"}},
        {Component{"Schacht"}, Component{"Stop"}}};

    // second part of tech_constraints needs to be sorted
    std::unordered_map<Component, std::vector<Subassembly>> tech_constraints{{Component{"Punt"}, std::vector<Subassembly>{std::vector<Component>{Component{"Inkt"}, Component{"Cartridge"}}}}};

    DiGraph<Component> obstr_graph_posX{obstruction_edges_posX};
    DiGraph<Component> obstr_graph_negX{obstr_graph_posX.reverse()};
    DiGraph<Component> obstr_graph_posY{obstruction_edges_posY};
    DiGraph<Component> obstr_graph_negY{obstr_graph_posY.reverse()};
    DiGraph<Component> obstr_graph_posZ{obstruction_edges_posZ};
    DiGraph<Component> obstr_graph_negZ{obstr_graph_posZ.reverse()};

    std::vector<DiGraph<Component>> obstr_graphs{obstr_graph_posX, obstr_graph_posY, obstr_graph_posZ, obstr_graph_negX, obstr_graph_negY, obstr_graph_negZ};
    Assembly assm{obstr_graphs, connection_graph, tech_constraints};

    Pomdp pomdp{"HRC assembly", assm};
    // pomdp.convert_and_solve("/home/cramer/Documents/programming/hrc_pomdp/output");
    pomdp.import_policy("/home/student/Documents/hrc_pomdp/output/hrc_assembly.policy");

    BayesFilter bf{pomdp.get_state_trans_probabilities(), pomdp.get_observation_probabilities(), pomdp.get_init_belief()};

    std::cout << "\nPress ENTER to start" << std::endl;
    std::cin.ignore();

    std::vector<Action> actions{pomdp.get_actions()};
    std::unordered_map<int, int> action_id_mapping{init_action_ids(actions)};

    std::vector<Observation> observations{pomdp.get_observations()};

    int num_assm_components{};
    while (num_assm_components < assm.get_components().size())
    {
        std::vector<double> current_belief{bf.get_belief()};

        // Select robot action using POMDP
        Action robot_action{pomdp.get_optimal_action(current_belief)};
        auto it_act = std::find(actions.begin(), actions.end(), robot_action);
        long int robot_action_id{std::distance(actions.begin(), it_act)};

        std::stringstream ss_robot_action{};
        std::string wait{"False"};
        if (action_id_mapping.at(robot_action_id) == 0) // wait
        {
            ss_robot_action << "";
            wait = "True";

            std::cout << "[SYSTEM]: Ordering robot to wait."
                      << std::endl;
        }
        else
        {
            ss_robot_action << action_id_mapping.at(robot_action_id);

            std::ostringstream ss_robot_action{};
            for (const Subassembly &subasm : robot_action.get_preconditions())
            {
                ss_robot_action << " [ ";
                for (const Component &comp : subasm)
                    ss_robot_action << comp.get_name() << " ";
                ss_robot_action << "] ";
            }

            std::cout << "[SYSTEM]: Commanding robot to assemble " << ss_robot_action.str()
                      << std::endl;
        }

        std::cout << "[SYSTEM]: Waiting for task execution update from robot ..."
                  << std::endl;

        // Execute robot action
        std::ostringstream ss_action_cmd{};
        ss_action_cmd << "python ~/catkin_ws/src/hrc_demonstrator/src/hrc_actionClient.py"
                      << " '" << ss_robot_action.str() << "' " << wait << " False";
        std::system(ss_action_cmd.str().c_str());

        std::cout << "[ROBOT]: Task completed successfully."
                  << std::endl;

        std::cout
            << "\n[SYSTEM]: Observing human operator ..."
            << std::endl;

        // Observe human action
        std::string s_observation = exec("python ~/catkin_ws/src/hrc_demonstrator/src/hrc_actionClient.py '' False True");

        std::vector<Component> manip_components{};
        bool manip_tool{};
        for (std::string &s_component : utils::split_string<std::string>(s_observation, ' '))
        {
            s_component.erase(std::remove_if(s_component.begin(), s_component.end(), isspace), s_component.end());

            if (s_component == "Hamer")
                manip_tool = true;
            else if (!s_component.empty())
            {
                manip_components.push_back(Component{s_component});
                num_assm_components++;
            }
        }

        if (!manip_components.empty())
            std::sort(manip_components.begin(), manip_components.end());

        Observation observation{manip_components, manip_tool};

        auto it_obs = std::find(observations.begin(), observations.end(), observation);
        long int observation_id{std::distance(observations.begin(), it_obs)};

        if (manip_components.empty() && !manip_tool)
        {
            std::cout << "[SYSTEM]: Human operator is waiting."
                      << std::endl;
        }
        else if (it_obs != observations.end())
        {
            std::vector<std::string> s_manip_components{};
            for (const Component &comp : manip_components)
                s_manip_components.push_back(comp.get_name());

            std::cout << "[SYSTEM]: Human operator is using the tool to assemble "
                      << utils::vector_to_string<std::string>(s_manip_components, ", ")
                      << std::endl;
        }
        else
        {
            std::cout << "!!! Assembly action not allowed !!!"
                      << std::endl;
            return -1;
        }

        // Update belief
        bf.update_belief(robot_action_id, observation_id);

        std::cout << std::endl;
    }

    return 0;
}