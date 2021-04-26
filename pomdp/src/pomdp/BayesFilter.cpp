#include <algorithm> // std::transform
#include <iostream>  // std::cerr
#include <numeric>   // std::accumulate
#include <vector>    // std::vector

#include <pomdp/BayesFilter.hpp>

BayesFilter::BayesFilter(const std::vector<std::vector<std::vector<double>>> &state_transition_cpt,
                         const std::vector<std::vector<std::vector<double>>> &measurement_cpt,
                         const std::vector<double> &init_belief)
    : num_states{}, num_controls{}, num_measurements{}, current_belief{init_belief},
      state_transition_cpt{state_transition_cpt}, measurement_cpt{measurement_cpt}, init_belief{init_belief}
{
    if (this->state_transition_cpt.empty() || this->measurement_cpt.empty() || this->init_belief.empty())
        std::cerr << "[BayesFilter] One or more model parameters are empty"
                  << std::endl;

    this->num_states = this->state_transition_cpt.size();
    this->num_controls = this->state_transition_cpt.at(0).size();
    this->num_measurements = this->measurement_cpt.at(0).at(0).size();
}

std::vector<double> BayesFilter::_prediction(const std::vector<double> &prior_belief, int control_id) const
{
    std::vector<double> posterior_belief(this->num_states);

    for (int next_state_id{0}; next_state_id < this->num_states; ++next_state_id)
    {
        double new_state_belief{};

        for (int current_state_id{0}; current_state_id < this->num_states; ++current_state_id)
            new_state_belief += this->state_transition_cpt.at(current_state_id).at(control_id).at(next_state_id) * prior_belief.at(current_state_id);

        posterior_belief.at(next_state_id) = new_state_belief;
    }

    return posterior_belief;
}

std::vector<double> BayesFilter::_correction(const std::vector<double> &prior_belief, int control_id, int measurement_id) const
{
    std::vector<double> posterior_belief(this->num_states);

    for (int state_id{0}; state_id < this->num_states; ++state_id)
        posterior_belief.at(state_id) = this->measurement_cpt.at(state_id).at(control_id).at(measurement_id) * prior_belief.at(state_id);

    double norm_factor{std::accumulate(posterior_belief.begin(), posterior_belief.end(), 0.0)};
    std::transform(posterior_belief.begin(), posterior_belief.end(), posterior_belief.begin(),
                   [&norm_factor](double state_belief) { return state_belief / norm_factor; });

    return posterior_belief;
}

std::vector<double> BayesFilter::get_belief() const
{
    return this->current_belief;
}

void BayesFilter::update_belief(int control_id, int measurement_id)
{
    std::vector<double> interm_belief{this->_prediction(this->current_belief, control_id)};
    std::vector<double> new_belief{this->_correction(interm_belief, control_id, measurement_id)};
    this->current_belief = new_belief;
}

void BayesFilter::set_belief(const std::vector<double> &belief)
{
    this->current_belief = belief;
}

void BayesFilter::reset_belief()
{
    this->set_belief(this->init_belief);
}