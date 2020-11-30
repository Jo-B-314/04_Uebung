#include <gtest/gtest.h>

#include <memory>
#include <initializer_list>
#include <iostream>
#include <set>
#include <vector>
#include <deque>
#include <string>

#include "../Sequence.h"
#include "../DNA.h"
#include "../Fastq.h"

using namespace Alphabet;

TEST(Fastq, writer)
{
    Sequence<DNA> Dnaseq;
    for (int i = 0; i < 25; i++) {
        Dnaseq.push_back(DNA::Characters::T);
    }
    Dnaseq.setComment("test");
    Sequence<DNA>& seq = Dnaseq;
    const Sequence<DNA> cseq = seq;
    SequenceFastqWriter<DNA> writer (cseq);
    std::string qvalstr;
    for (int i = 0; i < 25; i++) {
        seq.getQValues().push_back('a'); 
        qvalstr.push_back('a');
    }
    qvalstr.push_back('\n');
    std::string str = "@test\n" + seq.toString() + "\n" + "+test\n" + qvalstr ;
    std::stringstream ss;
    ss << toFastq(seq);
    std::cout << ss.str() << std::endl;
    ASSERT_EQ(0, str.compare(ss.str()));
}

TEST(Fastq, reader)
{
     Sequence<DNA> Dnaseq;
    Dnaseq.push_back(DNA::Characters::T);
    Dnaseq.push_back(DNA::Characters::A);
    Dnaseq.push_back(DNA::Characters::C);
    Dnaseq.setComment("test");
    const Sequence<DNA>& seq = Dnaseq;
    std::string stream_str = "@test\n" + seq.toString() + "+test\n";
    Sequence<DNA> emptyseq;
	SequenceFastqReader<DNA> reader (emptyseq);
    std::stringstream ss (stream_str);
    ss >> fromFastq(emptyseq);
	ASSERT_EQ(0, seq.toString().compare(reader.getSequence().toString()));
}
