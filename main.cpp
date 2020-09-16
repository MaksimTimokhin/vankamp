#include <graphviz/gvc.h>

#include <diagram_gen.hpp>
#include <graph/labeled_graph.hpp>
#include <iostream>
#include <parser/parser.hpp>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: vankamp <filename>\n";
        return 1;
    }

    std::string presentation;
    std::getline(std::cin, presentation);

    Parser parser;
    auto group = parser.ParseFromString(presentation);

    RandomVanKampenGenerator gen(*group);

    auto diagram = gen.GenerateRandomDiagram();

    std::cout << "\nWord: " << diagram.word << "\n\n";
    std::cout << "Dot description: \n" << diagram.diagram.ToDot() << '\n';

    diagram.diagram.ToPng(argv[1]);
    std::cout << "Visualization saved to " << argv[1] << '\n';

    return 0;
}
