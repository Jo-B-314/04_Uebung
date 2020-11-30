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
    Dnaseq.push_back(DNA::Characters::T);
    Dnaseq.push_back(DNA::Characters::A);
    Dnaseq.push_back(DNA::Characters::C);
    Dnaseq.setComment("test");
    Sequence<DNA>& seq = Dnaseq;
    const Sequence<DNA> cseq = seq;
    SequenceFastqWriter<DNA> writer (cseq);
    for (int i = 0; i < 3; i++) {
        seq.getQValues().push_back('a'); 
    }
    std::string qvalstr = "aaa\n";
    std::string str = "@test\n" + seq.toString() + "\n" + "+test\n" + qvalstr ;
    std::stringstream ss;
    ss << toFastq(seq);
    std::cout << ss.str() << std::endl;
    ASSERT_EQ(0, str.compare(ss.str()));

}
