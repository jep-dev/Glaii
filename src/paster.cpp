#include "streams.hpp"
#include "paster.hpp"
#include <algorithm>

namespace Streams {

		Paster& Paster::flush(size_t toCol, size_t toRow) {
			toCol = std::accumulate(data.cbegin(), data.cend(), toCol,
				[] (size_t lhs, string const& rhs) -> size_t {
					return std::max(lhs, rhs.size());
				});
			std::for_each(data.begin(), data.end(),
				[=] (string& src) -> string& {
				if(toCol > src.size())
					return src += string(toCol-src.size(), pad);
				});
			while(data.size() < toRow)
				data.emplace_back(toCol, pad);
			return *this;
		}
		Paster& Paster::operator<<(string const& rhs) {
			std::vector<string> sp;
			std::istringstream ss(rhs);
			while(ss) {
				string buf;
				if(getline(ss, buf, '\n')) {
					sp.emplace_back(buf);
				}
			}
			flush(0, sp.size());
			auto dBeg = data.begin();
			for(auto const& el : sp) {
				if(dBeg == data.end()) break;
				*dBeg += el;
				dBeg++;
			}
			return flush();
		}
		Paster& Paster::operator<<(ostringstream const& rhs) {
			return (*this) << rhs.str();
		}
}
