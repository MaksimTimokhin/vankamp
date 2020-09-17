#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <graphviz/gvc.h>

struct LabeledEdge {
    LabeledEdge() {
    }
    LabeledEdge(size_t to, std::string label) : to(to), label(std::move(label)) {
    }
    size_t to;
    std::string label;
};

class LabeledGraph {
public:
    LabeledGraph(size_t vertices_count) : outgoing_(vertices_count) {
    }

    size_t Size() const {
        return outgoing_.size();
    }

    void Resize(size_t vertices_count) {
        outgoing_.resize(vertices_count);

        if (vertices_count >= outgoing_.size()) {
            return;
        }

        for (auto &edges : outgoing_) {
            int first_bad = -1;
            for (int i = 0; i < edges.size(); ++i) {
                if (edges[i].to < outgoing_.size() && first_bad != -1) {
                    std::swap(edges[i], edges[first_bad]);
                    ++first_bad;
                } else if (edges[i].to >= outgoing_.size() && first_bad == -1) {
                    first_bad = i;
                }
            }
            edges.resize(first_bad);
        }
    }

    void AddEdge(size_t from, size_t to, const std::string &label) {
        outgoing_[from].emplace_back(to, label);
    }

    std::string ToDot(const std::string &name = "g") {
        std::stringstream description;
        description << "digraph " << name << "{\n";
        for (size_t i = 0; i < outgoing_.size(); ++i) {
            description << "  node" << i << " [label=\"\"" << (i == 0 ? ",style=filled" : "") << "]\n";
            for (const auto &edge : outgoing_[i]) {
                description << "  node" << i << "->"
                            << "node" << edge.to << "[label=\"" << edge.label << "\"]\n";
            }
        }
        description << '}';
        return description.str();
    }

    void ToPng(const std::string &filename) {
        FILE *output = fopen(filename.data(), "w");
        GVC_t *gv_context = gvContext();

        auto dot_description = ToDot();
        Agraph_t *graph = agmemread(dot_description.data());

        gvLayout(gv_context, graph, "dot");
        gvRender(gv_context, graph, "png", output);

        gvFreeLayout(gv_context, graph);
        agclose(graph);
        gvFreeContext(gv_context);
    }

private:
    std::vector<std::vector<LabeledEdge>> outgoing_;
};