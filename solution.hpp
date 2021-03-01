#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <regex>

struct Node
{
    int id;
    int next;
    int time_elapsed;

    std::string to_string() const
    {
        return "id: " + std::to_string(id) + " next: " + std::to_string(next) + " time_elapsed: " + std::to_string(time_elapsed);
    }
};

struct ChainInfo
{
    int last_job, start_job, number_of_jobs, time_elapsed;

    static std::string seconds_to_string(int s)
    {
        int hours, minutes, seconds;

        hours = s / 3600;
        s -= hours * 3600;
        minutes = s / 60;
        s -= minutes * 60;
        seconds = s;

        char buffer[50];
        sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
        return std::string(buffer);
    }

    void print(std::ostream &out) const
    {
        out << "start_job: " << start_job << std::endl;
        out << "last_job: " << last_job << std::endl;
        out << "number_of_jobs: " << number_of_jobs << std::endl;
        out << "job_chain_runtime: " << ChainInfo::seconds_to_string(time_elapsed) << std::endl;
        out << "average_job_time: " << ChainInfo::seconds_to_string(time_elapsed / number_of_jobs) << std::endl;
        out << "-" << std::endl;
    }
};

class Processor
{
    std::map<int, Node> nodes;
    std::set<int> was_next;
    std::set<int> all_ids;

    std::vector<int> GetBegins()
    {
        std::vector<int> begin_ids;
        std::set_difference(all_ids.begin(),
                            all_ids.end(),
                            was_next.begin(),
                            was_next.end(),
                            std::inserter(begin_ids, begin_ids.begin()));
        return begin_ids;
    }

    ChainInfo GetChainInfo(Node first_node)
    {
        ChainInfo res;

        res.start_job = first_node.id;
        res.number_of_jobs = 1;
        res.time_elapsed = first_node.time_elapsed;

        Node curr_node = first_node;

        std::set<int> visited;
        visited.insert(curr_node.id);

        while (curr_node.next != 0)
        {
            if (visited.find(curr_node.next) != visited.end())
            {
                throw std::runtime_error("cycle detected");
            }
            visited.insert(curr_node.next);

            auto it = nodes.find(curr_node.next);
            if (it == nodes.end())
            {
                throw std::runtime_error("next not found");
            }

            curr_node = it->second;
            res.number_of_jobs++;
            res.time_elapsed += curr_node.time_elapsed;
        }

        res.last_job = curr_node.id;

        return res;
    }

public:
    Processor(std::istream &in)
    {
        std::string line;
        std::getline(in, line);
        if (line != "#job_id,runtime_in_seconds,next_job_id")
        {
            throw std::runtime_error("wrong header");
        }

        std::regex line_regex("([^,]+),([^,]+),([^,]+)");
        std::smatch m;

        while (std::getline(in, line))
        {
            if (!std::regex_search(line, m, line_regex))
            {
                throw std::runtime_error("line malformed");
            }

            Node n;
            n.id = std::stoi(m[1]);
            n.time_elapsed = std::stoi(m[2]);
            n.next = std::stoi(m[3]);

            if (all_ids.find(n.id) != all_ids.end())
            {
                throw std::runtime_error("ids should be unique");
            }
            all_ids.insert(n.id);
            if (n.next != 0)
            {
                was_next.insert(n.next);
            }

            nodes[n.id] = n;
        }
    }

    std::vector<ChainInfo> GetChainInfos()
    {
        std::vector<ChainInfo> chain_infos;
        auto begin_ids = GetBegins();

        for (auto el : begin_ids)
        {
            chain_infos.push_back(GetChainInfo(nodes[el]));
        }
        return chain_infos;
    }
};

inline void print_map(std::map<int, Node> map)
{
    for (auto it = map.begin(); it != map.end(); it++)
    {
        std::cerr << it->first << " => " << it->second.to_string() << std::endl;
    }
}

inline void print_vector(std::vector<int> v)
{
    for (auto it = v.begin(); it != v.end(); it++)
    {
        std::cerr << *it << std::endl;
    }
}

inline void solution(std::istream &in = std::cin, std::ostream &out = std::cout)
{
    try
    {
        Processor p(in);
        auto chain_infos = p.GetChainInfos();

        out << "-" << std::endl;
        for (const auto &ci : chain_infos)
        {
            ci.print(out);
        }
    }
    catch (...)
    {
        out << "Malformed Input" << std::endl;
    }
}