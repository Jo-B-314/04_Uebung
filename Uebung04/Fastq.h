
#ifndef BIOINFOI_FASTQ_H
#define BIOINFOI_FASTQ_H

#include "Sequence.h"
#include "ParsingException.h"
#include "DNA.h"

#include <istream>
#include <ostream>
#include <iterator>

/**
 * A simple helper class used to signal
 * that we want to write a DNA sequence in 2Bit format.
 */
template<typename Alpha>
class SequenceFastqWriter
{
  public:
	SequenceFastqWriter(const Sequence<Alpha>& seq);
	
	Sequence<Alpha> getSequence() const{
		return seq_;
	}

	/// Global input operator
	template<typename A>
	friend std::ostream& operator<<(std::ostream& strm,
	                                const SequenceFastqWriter<A>&);


  private:
	/// Stores a reference to the sequence to write.
	const Sequence<Alpha>& seq_;
};

/**
 * A simple helper class used to signal
 * that we want to read a DNA sequence in 2Bit format.
 */
template<typename Alpha>
class SequenceFastqReader
{
  public:
	SequenceFastqReader(Sequence<Alpha>& seq);

	Sequence<Alpha>& getSequence() {
		return seq_;
	}

	/// Global input operator (Needs an rvalue reference to work with from2Bit...)
	template<typename A>
	friend std::istream& operator>>(std::istream& strm, SequenceFastqReader<A>&&);

  private:
	/// Stores a reference to the sequence to read.
	Sequence<Alpha>& seq_;
};

template<typename Alpha>
SequenceFastqWriter<Alpha> toFastq(const Sequence<Alpha>& seq) {
	SequenceFastqWriter<Alpha> writer(seq);
	return writer;
}

template<typename Alpha>
SequenceFastqReader<Alpha> fromFastq(Sequence<Alpha>& seq) {
	SequenceFastqReader<Alpha> reader(seq);
	return reader;
}

template <typename Alpha>
SequenceFastqReader<Alpha>::SequenceFastqReader(Sequence<Alpha>& seq)
    : seq_(seq)
{
}

template <typename Alpha>
std::ostream& operator<<(std::ostream& strm,
                         const SequenceFastqWriter<Alpha>& writer)
{
	Sequence<Alpha> seq_ = writer.getSequence();
            strm << "@" << seq_.getComment() << std::endl;
            auto iter = seq_.begin();
            int s = 0;
            for (; iter != seq_.end(); iter++) {
                if (*iter == Alphabet::DNA::Characters::N) {
                    break;
                }
                s++;
                strm << Alphabet::DNA::toChar(*iter);
            }
            strm << std::endl;
            strm << "+" << seq_.getComment() << std::endl;
            std::vector<char>& qvalues = seq_.getQValues();
            for (int i = 0; i < s; i++) {
                strm << qvalues[i];
            }
            strm << std::endl;
        	return strm;
}

template <typename Alpha>
std::istream& operator>>(std::istream& strm,
                         SequenceFastqReader<Alpha>&& reader)
{
	std::istream_iterator<char> eos; //end of stream iterator
	std::string comment;
	char c;
	strm >> c;
	if (c == '@') {
		//there is a comment -> we read the header line
		getline(strm, comment);
		reader.getSequence().setComment(comment);
	} else {
		throw ParsingException("no Header");
	}
	std::istream_iterator<char> iter(strm);
	if (iter == eos) {
		throw ParsingException("Header but no Sequence");
	}
	Sequence<Alpha>& seq = reader.getSequence();
	for (; (*iter != '+') && (iter != eos); iter++) {
		seq.push_back(Alpha::toCharacter(*iter));
	}
	if (iter == eos) throw ParsingException("no line 3");
    getline(strm, comment);
    int i = 0;
    while (((char) strm.peek() != '\n') && strm.good()) {
        strm >> c;
        // c< 63 <=> P > 0.001
        if (c < 63) {
            seq[i] = Alphabet::DNA::Characters::N;
        }
        i++;
    }
	while ((char) strm.peek() == '\n') strm.get();
	return strm;
}

template <typename Alpha>
SequenceFastqWriter<Alpha>::SequenceFastqWriter(const Sequence<Alpha>& seq)
    : seq_(seq)
{
}

#endif // BIOINFOI_FASTA_H

