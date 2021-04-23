// Terminology adopted from: Probabilistic Robotics - Thrun, Burgard, and Fox (2005)

#ifndef BAYES_FILTER_HPP
#define BAYES_FILTER_HPP

#include <vector> // std::vector

class BayesFilter
{
private:
    int num_states;       // x
    int num_controls;     // u
    int num_measurements; // z
    std::vector<double> current_belief;

    std::vector<std::vector<std::vector<double>>> state_transition_cpt; // p(x_t | x_t-1, u_t)
    std::vector<std::vector<std::vector<double>>> measurement_cpt;      // p(z_t | x_t, u_t)
    std::vector<double> init_belief;

    std::vector<double> _prediction(const std::vector<double> &prior_belief, int control_id) const;
    std::vector<double> _correction(const std::vector<double> &prior_belief, int control_id, int measurement_id) const;

public:
    explicit BayesFilter(const std::vector<std::vector<std::vector<double>>> &state_transition_cpt,
                         const std::vector<std::vector<std::vector<double>>> &measurement_cpt,
                         const std::vector<double> &init_belief);
    ~BayesFilter() = default;

    std::vector<double> get_belief() const;
    void set_belief(const std::vector<double> &belief);

    void update_belief(int control_id, int measurement_id);
    void reset_belief();
};

#endif // BAYES_FILTER_HPP