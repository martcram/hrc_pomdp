#ifndef POMDP_HPP
#define POMDP_HPP

#include <map>    // std::map
#include <string> // std::string
#include <vector> // std::vector

#include <graph/DiGraph.hpp>

#include <main/Assembly.hpp>
#include <main/Component.hpp>

#include <pomdp/Action.hpp>
#include <pomdp/Observation.hpp>

using Subassembly = std::vector<Component>;
using State = std::vector<Subassembly>;
using Intention = std::vector<State>;

class Pomdp
{
private:
    std::string description;
    std::string file_name;

    Assembly assembly;
    DiGraph<State, Action> state_graph;
    DiGraph<Intention, Action> intention_graph;

    bool import_action_ids;

    std::map<int, Intention> intention_ids;
    std::map<int, Action> action_ids;
    std::map<int, Observation> observation_ids;

    std::unordered_map<int, int> action_obs_mapping;

    int num_intentions;
    int num_actions;
    int num_observations;

    // POMDP model parameters
    std::vector<double> init_belief;
    std::vector<std::vector<std::vector<double>>> state_trans_probabilities;
    std::vector<std::vector<std::vector<double>>> observation_probabilities;
    std::vector<std::vector<double>> rewards;
    double discount;

    std::vector<Action> robot_actions;

    std::string pomdpx_file_path;
    std::string policy_file_path;
    std::unordered_map<int, std::vector<std::vector<double>>> policy;

    bool _get_id(const Intention &intention, int &out) const;
    bool _get_id(const Action &action, int &out) const;
    bool _get_id(const Observation &observation, int &out) const;

    bool _exist_file(const std::string &file_path) const;

    void _add_intention(const Intention &intention);
    void _add_action(const Action &action);
    void _add_action(const Action &action, int id);
    void _add_observation(const Observation &observation);

    void _init_belief();
    void _init_state_trans();
    void _init_observation_func();
    void _init_reward_func();

    DiGraph<State, Action> _generate_state_graph() const;
    DiGraph<Intention, Action> _generate_intention_graph() const;
    std::vector<Intention> _get_state_trans(const Intention &current_intention, const Action &action) const;
    std::vector<Action> _get_actions(const Intention &intention) const;
    std::vector<Action> _get_prev_actions(const Intention &intention) const;
    std::vector<Observation> _get_observations(const Intention &intention) const;

public:
    explicit Pomdp(const std::string &description);
    explicit Pomdp(const std::string &description, const Assembly &assembly, bool import_action_ids = false);
    ~Pomdp() = default;

    std::string get_description() const;
    std::vector<Intention> get_intentions() const;
    std::vector<Action> get_actions() const;
    std::vector<Observation> get_observations() const;

    DiGraph<State, Action> get_state_graph() const;
    DiGraph<Intention, Action> get_intention_graph() const;

    std::vector<double> get_init_belief() const;
    std::vector<std::vector<std::vector<double>>> get_state_trans_probabilities() const;
    std::vector<std::vector<std::vector<double>>> get_observation_probabilities() const;
    std::vector<std::vector<double>> get_rewards() const;
    double get_discount() const;

    int get_num_states() const;
    int get_num_actions() const;
    int get_num_observations() const;

    void convert_to_pomdpx(const std::string &output_loc);
    void solve();
    void convert_and_solve(const std::string &output_loc);

    void import_pomdpx(const std::string &file_path);
    void import_policy(const std::string &file_path);

    Action get_optimal_action(const std::vector<double> &belief) const;
};

#endif // POMDP_HPP