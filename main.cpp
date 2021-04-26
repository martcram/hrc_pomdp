#include <iostream>
#include <vector>

#include <graph/DiGraph.hpp>
#include <graph/Graph.hpp>

#include <main/Assembly.hpp>
#include <main/Component.hpp>

#include <pomdp/BayesFilter.hpp>
#include <pomdp/Pomdp.hpp>

using Subassembly = std::vector<Component>;

int main()
{
    std::vector<std::pair<Component, Component>> liaison_edges{
        {Component{"Ink"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Head"}},
        {Component{"Head"}, Component{"Body"}},
        {Component{"Body"}, Component{"Cap"}},
        {Component{"Body"}, Component{"Button"}}};

    Graph<Component> connection_graph{liaison_edges};

    std::vector<std::pair<Component, Component>> obstruction_edges_posX{
        {Component{"Cap"}, Component{"Ink"}},
        {Component{"Cap"}, Component{"Cartridge"}},
        {Component{"Cap"}, Component{"Head"}},
        {Component{"Cap"}, Component{"Body"}},
        {Component{"Cap"}, Component{"Button"}},
        {Component{"Head"}, Component{"Cartridge"}},
        {Component{"Head"}, Component{"Button"}},
        {Component{"Head"}, Component{"Body"}},
        {Component{"Head"}, Component{"Ink"}},
        {Component{"Cartridge"}, Component{"Button"}},
        {Component{"Body"}, Component{"Button"}},
        {Component{"Ink"}, Component{"Button"}}};

    std::vector<std::pair<Component, Component>> obstruction_edges_posY{
        {Component{"Head"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Head"}},
        {Component{"Head"}, Component{"Body"}},
        {Component{"Body"}, Component{"Head"}},
        {Component{"Head"}, Component{"Cap"}},
        {Component{"Cap"}, Component{"Head"}},
        {Component{"Cartridge"}, Component{"Ink"}},
        {Component{"Ink"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Body"}},
        {Component{"Body"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Cap"}},
        {Component{"Cap"}, Component{"Cartridge"}},
        {Component{"Ink"}, Component{"Body"}},
        {Component{"Body"}, Component{"Ink"}},
        {Component{"Ink"}, Component{"Cap"}},
        {Component{"Cap"}, Component{"Ink"}},
        {Component{"Cap"}, Component{"Body"}},
        {Component{"Body"}, Component{"Cap"}},
        {Component{"Button"}, Component{"Body"}},
        {Component{"Body"}, Component{"Button"}}};

    std::vector<std::pair<Component, Component>> obstruction_edges_posZ{
        {Component{"Head"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Head"}},
        {Component{"Head"}, Component{"Body"}},
        {Component{"Body"}, Component{"Head"}},
        {Component{"Head"}, Component{"Cap"}},
        {Component{"Cap"}, Component{"Head"}},
        {Component{"Cartridge"}, Component{"Ink"}},
        {Component{"Ink"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Body"}},
        {Component{"Body"}, Component{"Cartridge"}},
        {Component{"Cartridge"}, Component{"Cap"}},
        {Component{"Cap"}, Component{"Cartridge"}},
        {Component{"Ink"}, Component{"Body"}},
        {Component{"Body"}, Component{"Ink"}},
        {Component{"Ink"}, Component{"Cap"}},
        {Component{"Cap"}, Component{"Ink"}},
        {Component{"Cap"}, Component{"Body"}},
        {Component{"Body"}, Component{"Cap"}},
        {Component{"Button"}, Component{"Body"}},
        {Component{"Body"}, Component{"Button"}}};

    // second part of tech_constraints needs to be sorted
    std::unordered_map<Component, std::vector<Subassembly>> tech_constraints{{Component{"Head"}, std::vector<Subassembly>{std::vector<Component>{Component{"Ink"}, Component{"Cartridge"}}}}};

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
    pomdp.import_policy("/home/cramer/Documents/programming/hrc_pomdp/output/hrc_assembly.policy");

    BayesFilter bf{pomdp.get_state_trans_probabilities(), pomdp.get_observation_probabilities(), pomdp.get_init_belief()};

    std::cout << "\nPress ENTER to start" << std::endl;
    std::cin.ignore();

    int num_assm_parts{};
    while (num_assm_parts < assm.get_components().size())
    {
        std::vector<double> current_belief{bf.get_belief()};

        // Select robot action from POMDP
        Action robot_action{pomdp.get_optimal_action(current_belief)};

        // Execute robot action
    }

    return 0;
}