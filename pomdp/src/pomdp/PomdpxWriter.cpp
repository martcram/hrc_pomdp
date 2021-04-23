#include <sstream> // std::ostringstream
#include <string>  // std::string

#include <pomdp/Pomdp.hpp>
#include <pomdp/PomdpxWriter.hpp>

#include "tinyxml2.h"

#include <utils/utils.hpp>

PomdpxWriter::PomdpxWriter(const Pomdp &pomdp)
    : pomdp{pomdp},
      xml_doc{}, root_ptr{},
      state_var_tag{"state"}, obs_var_tag{"obs"}, action_var_tag{"action"}, reward_var_tag{"reward"}
{
    tinyxml2::XMLDeclaration *decl_ptr = this->xml_doc.NewDeclaration("xml version=\"1.0\" encoding=\"ISO-8859-1\"");
    this->xml_doc.InsertFirstChild(decl_ptr);

    this->root_ptr = this->xml_doc.NewElement("pomdpx");
    this->xml_doc.InsertEndChild(this->root_ptr);

    this->_gen_header();
    this->_gen_description();
    this->_gen_discount();
    this->_gen_variables();
    this->_gen_init_belief();
    this->_gen_state_trans_func();
    this->_gen_obs_func();
    this->_gen_reward_func();
}

void PomdpxWriter::_gen_header()
{
    this->root_ptr->SetAttribute("version", "0.1");
    this->root_ptr->SetAttribute("id", "HRC_Assembly");
    this->root_ptr->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    this->root_ptr->SetAttribute("xsi:noNamespaceSchemaLocation", "pomdpx.xsd");
    //this->xml_doc.InsertEndChild(this->root_ptr);
}

void PomdpxWriter::_gen_description()
{
    // Tag: Description
    tinyxml2::XMLElement *description_ptr = this->xml_doc.NewElement("Description");
    description_ptr->SetText(this->pomdp.get_description().c_str());

    this->root_ptr->InsertEndChild(description_ptr);
}

void PomdpxWriter::_gen_discount()
{
    // Tag: Discount
    tinyxml2::XMLElement *discount_ptr = this->xml_doc.NewElement("Discount");
    discount_ptr->SetText(this->pomdp.get_discount());

    this->root_ptr->InsertEndChild(discount_ptr);
}

void PomdpxWriter::_gen_variables()
{
    // Tag: Variable
    tinyxml2::XMLNode *variable_ptr = this->xml_doc.NewElement("Variable");
    this->root_ptr->InsertEndChild(variable_ptr);

    // > Tag: StateVar
    tinyxml2::XMLElement *state_var_ptr = this->xml_doc.NewElement("StateVar");
    state_var_ptr->SetAttribute("vnamePrev", std::string{this->state_var_tag + "_0"}.c_str());
    state_var_ptr->SetAttribute("vnameCurr", std::string{this->state_var_tag + "_1"}.c_str());
    state_var_ptr->SetAttribute("fullyObs", "false");
    variable_ptr->InsertEndChild(state_var_ptr);

    // >> Tag: NumValues
    tinyxml2::XMLElement *state_values_ptr = this->xml_doc.NewElement("NumValues");
    state_values_ptr->SetText(this->pomdp.get_num_states());
    state_var_ptr->InsertEndChild(state_values_ptr);

    // > Tag: ObsVar
    tinyxml2::XMLElement *obs_var_ptr = this->xml_doc.NewElement("ObsVar");
    obs_var_ptr->SetAttribute("vname", this->obs_var_tag.c_str());
    variable_ptr->InsertEndChild(obs_var_ptr);

    // >> Tag: NumValues
    tinyxml2::XMLElement *obs_values_ptr = this->xml_doc.NewElement("NumValues");
    obs_values_ptr->SetText(this->pomdp.get_num_observations());
    obs_var_ptr->InsertEndChild(obs_values_ptr);

    // > Tag: ActionVar
    tinyxml2::XMLElement *action_var_ptr = this->xml_doc.NewElement("ActionVar");
    action_var_ptr->SetAttribute("vname", this->action_var_tag.c_str());
    variable_ptr->InsertEndChild(action_var_ptr);

    // >> Tag: NumValues
    tinyxml2::XMLElement *action_values_ptr = this->xml_doc.NewElement("NumValues");
    action_values_ptr->SetText(this->pomdp.get_num_actions());
    action_var_ptr->InsertEndChild(action_values_ptr);

    // > Tag: RewardVar
    tinyxml2::XMLElement *reward_var_ptr = this->xml_doc.NewElement("RewardVar");
    reward_var_ptr->SetAttribute("vname", this->reward_var_tag.c_str());
    variable_ptr->InsertEndChild(reward_var_ptr);
}

void PomdpxWriter::_gen_init_belief()
{
    // Tag: InitialStateBelief
    tinyxml2::XMLNode *init_belief_ptr = this->xml_doc.NewElement("InitialStateBelief");
    this->root_ptr->InsertEndChild(init_belief_ptr);

    // > Tag: CondProb
    tinyxml2::XMLNode *cond_prob_ptr = this->xml_doc.NewElement("CondProb");
    init_belief_ptr->InsertEndChild(cond_prob_ptr);

    // >> Tag: Var
    tinyxml2::XMLElement *var_ptr = this->xml_doc.NewElement("Var");
    var_ptr->SetText(std::string{this->state_var_tag + "_0"}.c_str());
    cond_prob_ptr->InsertEndChild(var_ptr);

    // >> Tag: Parent
    tinyxml2::XMLElement *parent_ptr = this->xml_doc.NewElement("Parent");
    parent_ptr->SetText("null");
    cond_prob_ptr->InsertEndChild(parent_ptr);

    // >> Tag: Parameter
    tinyxml2::XMLElement *param_ptr = this->xml_doc.NewElement("Parameter");
    param_ptr->SetAttribute("type", "TBL");
    cond_prob_ptr->InsertEndChild(param_ptr);

    // >>> Tag: Entry
    tinyxml2::XMLNode *entry_ptr = this->xml_doc.NewElement("Entry");
    param_ptr->InsertEndChild(entry_ptr);

    // >>>> Tag: Instance
    tinyxml2::XMLElement *instance_ptr = this->xml_doc.NewElement("Instance");
    instance_ptr->SetText("-");
    entry_ptr->InsertEndChild(instance_ptr);

    // >>>> Tag: ProbTable
    tinyxml2::XMLElement *prob_table_ptr = this->xml_doc.NewElement("ProbTable");
    prob_table_ptr->SetText(utils::vector_to_string(this->pomdp.get_init_belief(), " ").c_str());
    entry_ptr->InsertEndChild(prob_table_ptr);
}

void PomdpxWriter::_gen_state_trans_func()
{
    // Tag: StateTransitionFunction
    tinyxml2::XMLNode *state_trans_ptr = this->xml_doc.NewElement("StateTransitionFunction");
    this->root_ptr->InsertEndChild(state_trans_ptr);

    // > Tag: CondProb
    tinyxml2::XMLNode *cond_prob_ptr = this->xml_doc.NewElement("CondProb");
    state_trans_ptr->InsertEndChild(cond_prob_ptr);

    // >> Tag: Var
    tinyxml2::XMLElement *var_ptr = this->xml_doc.NewElement("Var");
    var_ptr->SetText(std::string{this->state_var_tag + "_1"}.c_str());
    cond_prob_ptr->InsertEndChild(var_ptr);

    // >> Tag: Parent
    tinyxml2::XMLElement *parent_ptr = this->xml_doc.NewElement("Parent");

    std::ostringstream ss_parent_vars{};
    ss_parent_vars << this->state_var_tag << "_0"
                   << " "
                   << this->action_var_tag;

    parent_ptr->SetText(ss_parent_vars.str().c_str());
    cond_prob_ptr->InsertEndChild(parent_ptr);

    // >> Tag: Parameter
    tinyxml2::XMLElement *param_ptr = this->xml_doc.NewElement("Parameter");
    param_ptr->SetAttribute("type", "TBL");
    cond_prob_ptr->InsertEndChild(param_ptr);

    std::vector<std::vector<std::vector<double>>> state_trans_prob{this->pomdp.get_state_trans_probabilities()};
    for (int curr_state_id{0}; curr_state_id < this->pomdp.get_num_states(); ++curr_state_id)
    {
        for (int action_id{0}; action_id < this->pomdp.get_num_actions(); ++action_id)
        {
            for (int next_state_id{0}; next_state_id < this->pomdp.get_num_states(); ++next_state_id)
            {
                // >>> Tag: Entry
                tinyxml2::XMLNode *entry_ptr = this->xml_doc.NewElement("Entry");
                param_ptr->InsertEndChild(entry_ptr);

                // >>>> Tag: Instance
                tinyxml2::XMLElement *instance_ptr = this->xml_doc.NewElement("Instance");

                std::ostringstream ss_instance{};
                ss_instance << "s" << curr_state_id << " "
                            << "a" << action_id << " "
                            << "s" << next_state_id;

                instance_ptr->SetText(ss_instance.str().c_str());
                entry_ptr->InsertEndChild(instance_ptr);

                // >>>> Tag: ProbTable
                tinyxml2::XMLElement *prob_table_ptr = this->xml_doc.NewElement("ProbTable");
                prob_table_ptr->SetText(state_trans_prob.at(curr_state_id).at(action_id).at(next_state_id));
                entry_ptr->InsertEndChild(prob_table_ptr);
            }
        }
    }
}

void PomdpxWriter::_gen_obs_func()
{
    // Tag: ObsFunction
    tinyxml2::XMLNode *obs_ptr = this->xml_doc.NewElement("ObsFunction");
    this->root_ptr->InsertEndChild(obs_ptr);

    // > Tag: CondProb
    tinyxml2::XMLNode *cond_prob_ptr = this->xml_doc.NewElement("CondProb");
    obs_ptr->InsertEndChild(cond_prob_ptr);

    // >> Tag: Var
    tinyxml2::XMLElement *var_ptr = this->xml_doc.NewElement("Var");
    var_ptr->SetText(this->obs_var_tag.c_str());
    cond_prob_ptr->InsertEndChild(var_ptr);

    // >> Tag: Parent
    tinyxml2::XMLElement *parent_ptr = this->xml_doc.NewElement("Parent");

    std::ostringstream ss_parent_vars{};
    ss_parent_vars << this->state_var_tag << "_1"
                   << " "
                   << this->action_var_tag;

    parent_ptr->SetText(ss_parent_vars.str().c_str());
    cond_prob_ptr->InsertEndChild(parent_ptr);

    // >> Tag: Parameter
    tinyxml2::XMLElement *param_ptr = this->xml_doc.NewElement("Parameter");
    param_ptr->SetAttribute("type", "TBL");
    cond_prob_ptr->InsertEndChild(param_ptr);

    std::vector<std::vector<std::vector<double>>> obs_prob{this->pomdp.get_observation_probabilities()};
    for (int state_id{0}; state_id < this->pomdp.get_num_states(); ++state_id)
    {
        for (int action_id{0}; action_id < this->pomdp.get_num_actions(); ++action_id)
        {
            for (int obs_id{0}; obs_id < this->pomdp.get_num_observations(); ++obs_id)
            {
                // >>> Tag: Entry
                tinyxml2::XMLNode *entry_ptr = this->xml_doc.NewElement("Entry");
                param_ptr->InsertEndChild(entry_ptr);

                // >>>> Tag: Instance
                tinyxml2::XMLElement *instance_ptr = this->xml_doc.NewElement("Instance");

                std::ostringstream ss_instance{};
                ss_instance << "s" << state_id << " "
                            << "a" << action_id << " "
                            << "o" << obs_id;

                instance_ptr->SetText(ss_instance.str().c_str());
                entry_ptr->InsertEndChild(instance_ptr);

                // >>>> Tag: ProbTable
                tinyxml2::XMLElement *prob_table_ptr = this->xml_doc.NewElement("ProbTable");
                prob_table_ptr->SetText(obs_prob.at(state_id).at(action_id).at(obs_id));
                entry_ptr->InsertEndChild(prob_table_ptr);
            }
        }
    }
}

void PomdpxWriter::_gen_reward_func()
{
    // Tag: RewardFunction
    tinyxml2::XMLNode *reward_ptr = this->xml_doc.NewElement("RewardFunction");
    this->root_ptr->InsertEndChild(reward_ptr);

    // > Tag: Func
    tinyxml2::XMLNode *func_ptr = this->xml_doc.NewElement("Func");
    reward_ptr->InsertEndChild(func_ptr);

    // >> Tag: Var
    tinyxml2::XMLElement *var_ptr = this->xml_doc.NewElement("Var");
    var_ptr->SetText(this->reward_var_tag.c_str());
    func_ptr->InsertEndChild(var_ptr);

    // >> Tag: Parent
    tinyxml2::XMLElement *parent_ptr = this->xml_doc.NewElement("Parent");

    std::ostringstream ss_parent_vars{};
    ss_parent_vars << this->state_var_tag << "_0"
                   << " "
                   << this->action_var_tag;

    parent_ptr->SetText(ss_parent_vars.str().c_str());
    func_ptr->InsertEndChild(parent_ptr);

    // >> Tag: Parameter
    tinyxml2::XMLElement *param_ptr = this->xml_doc.NewElement("Parameter");
    param_ptr->SetAttribute("type", "TBL");
    func_ptr->InsertEndChild(param_ptr);

    std::vector<std::vector<double>> rewards{this->pomdp.get_rewards()};
    for (int state_id{0}; state_id < this->pomdp.get_num_states(); ++state_id)
    {
        for (int action_id{0}; action_id < this->pomdp.get_num_actions(); ++action_id)
        {
            // >>> Tag: Entry
            tinyxml2::XMLNode *entry_ptr = this->xml_doc.NewElement("Entry");
            param_ptr->InsertEndChild(entry_ptr);

            // >>>> Tag: Instance
            tinyxml2::XMLElement *instance_ptr = this->xml_doc.NewElement("Instance");

            std::ostringstream ss_instance{};
            ss_instance << "s" << state_id << " "
                        << "a" << action_id;

            instance_ptr->SetText(ss_instance.str().c_str());
            entry_ptr->InsertEndChild(instance_ptr);

            // >>>> Tag: ProbTable
            tinyxml2::XMLElement *prob_table_ptr = this->xml_doc.NewElement("ValueTable");
            prob_table_ptr->SetText(rewards.at(state_id).at(action_id));
            entry_ptr->InsertEndChild(prob_table_ptr);
        }
    }
}

void PomdpxWriter::write(const std::string &file_path)
{
    this->xml_doc.SaveFile(file_path.c_str());
}