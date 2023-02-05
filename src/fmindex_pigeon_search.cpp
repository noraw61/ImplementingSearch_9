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

    auto reference_file = std::filesystem::path{};
    parser.add_option(reference_file, '\0', "reference", "path to the reference file");

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
    auto reference_stream = seqan3::sequence_file_input{reference_file};
    
    //reference
    std::vector<std::vector<seqan3::dna5>> reference;
    for (auto& record : reference_stream) {
	    reference.push_back(record.sequence());
    }

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
    queries.resize(10); // will reduce the amount of searches


    for(auto& query : queries){
        //seqan3::debug_stream << "query: "<<query<<", length:"<< query.size()<<std::endl;
        unsigned subq_len = std::floor(query.size()/(k+1)); //length of subqueries berechnen
        //seqan3::debug_stream <<"length of subqueries: "<<subq_len << std::endl;
        for(unsigned i=0; i<k+1;++i){ //k+1 pieces
            auto subq = seqan3::views::slice(query,i*subq_len, (i+1)*subq_len);//alle teile gleich lang und evtl die letzten buchstaben nicht im letzten subquery sind
            //seqan3::debug_stream <<"\t\t Subquery "<<i<<" :"<<subq<<", length: "<<subq.size()<<std::endl;
            seqan3::configuration const cfg = seqan3::search_cfg::max_error_total{seqan3::search_cfg::error_count{0}}; 
            auto res_search = seqan3::search(subq,index, cfg);
            for(auto && result : res_search){
                //seqan3::debug_stream <<"\t\t"<< result << '\n';
                auto start_pos_sub = result.reference_begin_position(); 
                //seqan3::debug_stream << "\t\t start pos subquery:"<<start_pos_sub << '\n';
                auto start_pos = start_pos_sub-i*subq_len;
                //seqan3::debug_stream << "\t\t start pos query:"<<start_pos << '\n';
                //verifizieren (hamming distance)
                //seqan3::debug_stream << "\t\t\t Found subquery with FM-Index and 0 errors in this subtext:";
                unsigned counter = 0;
                //std::vector<seqan3::dna5> subtext{};
                for(int j = start_pos;j<start_pos+query.size(); ++j){
                    //seqan3::debug_stream <<reference[0][j];
                    //subtext.push_back(reference[0][j]);
                    if(reference[0][j] != query[j-start_pos]){ //compare nucleotides
                        counter+=1; //count errors
                        if(counter>k){
                            //    seqan3::debug_stream <<"\t\t\t\tbut not found whole query because of to many errors."<<std::endl;
                            break;
                        }
                    }
                }
                //seqan3::debug_stream <<'\n';
                if(counter <= k){
                    seqan3::debug_stream <<"\tfound whole query: "<<query<<" in reference at position: "<<start_pos<<" with "<<counter<<" errors."<<std::endl;
                }
        
            } 
        }
    }
    
    return 0;
}
