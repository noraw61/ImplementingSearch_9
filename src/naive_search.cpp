#include <sstream>
#include <math.h>
#include <map>
#include <omp.h>

#include <seqan3/std/filesystem>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>


void shiftOr(const std::vector<seqan3::dna5> & text, const std::vector<seqan3::dna5> pattern)
{
    //Preprocessing
    for(auto letter : pattern)
    {
        seqan3::debug_stream<<letter;
    }
    seqan3::debug_stream<<": "; 
    std::map<seqan3::dna5, uint64_t> Bitmask{};
    size_t m = pattern.size();
    size_t count = 0;
    uint64_t bits = pow(2,m)-1;
    uint64_t mask = bits;
    for(size_t i = 0; i < m; i++)
    {
        Bitmask[pattern[i]] = mask;
    }
    mask<<=1;
    mask&=bits;
    for(size_t i = 0; i < m; i++)
    {
        Bitmask[pattern[i]] = Bitmask[pattern[i]] & mask;
        mask<<=1;
        mask|=1;
    }
    uint64_t hit = bits >> 1;
    //search
    uint64_t D = bits;
    for(size_t i = 0; i < text.size(); i++)
    {
        D <<= 1;
        D &= bits;
        if(Bitmask.count(text[i]) != 0)
            D |= Bitmask[text[i]];
        else
            D |= bits;
        if((D|hit) != bits)
        {
            count++;
            seqan3::debug_stream<<i-m+1<<" ";
        }
    }
    seqan3::debug_stream << "\n";
}


// prints out all occurences of query inside of ref
void findOccurences(std::vector<seqan3::dna5> const& ref, std::vector<seqan3::dna5> const& query) {

    shiftOr(ref,query);
    //!TODO ImplementMe
}

int main(int argc, char const* const* argv) {
    seqan3::argument_parser parser{"naive_search", argc, argv, seqan3::update_notifications::off};

    parser.info.author = "SeqAn-Team";
    parser.info.version = "1.0.0";

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
    auto reference_stream = seqan3::sequence_file_input{reference_file};
    auto query_stream     = seqan3::sequence_file_input{query_file};

    // read reference into memory
    std::vector<std::vector<seqan3::dna5>> reference;
    for (auto& record : reference_stream) {
        reference.push_back(record.sequence());
    }

    // read query into memory
    std::vector<std::vector<seqan3::dna5>> queries;
    for (auto& record : query_stream) {
        queries.push_back(record.sequence());
    }

    //!TODO !CHANGEME here adjust the number of searches
    queries.resize(10000); // will reduce the amount of searches

    //! search for all occurences of queries inside of reference
    for (auto& r : reference) {
        for (auto& q : queries) {
            findOccurences(r, q);
        }
    }

    return 0;
}
