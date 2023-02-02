#include <sstream>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>

#include <cmath>
#include <vector>

int main(int argc, char const* const* argv) {
    seqan3::argument_parser parser{"fmindex_pigeon_search", argc, argv, seqan3::update_notifications::off};

    parser.info.author = "SeqAn-Team";
    parser.info.version = "1.0.0";

    auto index_path = std::filesystem::path{};
    parser.add_option(index_path, '\0', "index", "path to the query file");

    auto query_file = std::filesystem::path{};
    parser.add_option(query_file, '\0', "query", "path to the query file");

    try {
         parser.parse();
    } catch (seqan3::argument_parser_error const& ext) {
        seqan3::debug_stream << "Parsing error. " << ext.what() << "\n";
        return EXIT_FAILURE;
    }

    // loading our files
    auto query_stream     = seqan3::sequence_file_input{query_file};

    // read query into memory
    std::vector<std::vector<seqan3::dna5>> queries;
    for (auto& record : query_stream) {
        queries.push_back(record.sequence());
    }

    // loading fm-index into memory
    using Index = decltype(seqan3::fm_index{std::vector<std::vector<seqan3::dna5>>{}}); // Some hack
    Index index; // construct fm-index
    {
        seqan3::debug_stream << "Loading 2FM-Index ... " << std::flush;
        std::ifstream is{index_path, std::ios::binary};
        cereal::BinaryInputArchive iarchive{is};
        iarchive(index);
        seqan3::debug_stream << "done\n";
    }
    //we choose here our k
    unsigned k = 2;

    //!TODO here adjust the number of searches
    queries.resize(5); // will reduce the amount of searches


    //!TODO !ImplementMe use the seqan3::search to find a partial error free hit, verify the rest inside the text
    // Pseudo code (might be wrong):
    // for query in queries:
    //      parts[3] = cut_query(3, query);
    //      for p in {0, 1, 2}:
    //          for (pos in search(index, part[p]):
    //              if (verify(ref, query, pos +- ....something)):
    //                  std::cout << "found something\n"
    for(auto& query : queries){
        seqan3::debug_stream << "query: "<<query<<std::endl;
        unsigned subq_len = std::floor(query.size()/(k+1)); //length of subqueries berechnen
        seqan3::debug_stream <<"length of subqueries: "<<subq_len << std::endl;
        for(unsigned i=0; i<k+1;++i){ //k+1 pieces
            seqan3::debug_stream <<"Subquery "<<i<<" :"<<seqan3::views::slice(query,i*subq_len, (i+1)*subq_len)<<std::endl;
            std::vector<seqan3::dna5> subq = slice(query,i*subq_len, (i+1)*subq_len); //alle teile gleich lang und evtl die letzten buchstaben nicht im letzten subquery sind
            seqan3::configuration const cfg = seqan3::search_cfg::max_error_total{seqan3::search_cfg::error_count{0}};
            //seqan3::search_result res_search = seqan3::search(index, subq, cfg);
        //for(auto& pos : res_search){
          //  seqan3::configuration const cfg = seqan3::search_cfg::max_error_total{seqan3::search_cfg::error_count{k}};
            //std::integral start_pos_sub = pos.seqan3::search_result::reference_begin_position();
            //std::integral start_pos = start_pos_sub-i*subq_len;
            //seqan3::search_result res_complete = seqan3::search(index[start_pos, start_pos+query.size()], query, cfg); //das wäre jetzt aber ohne edit distance
        } 
      //}
    }
    
    return 0;
}
