#pragma once

#include "precompiled.hpp"

namespace mt
{
	struct Indenter
	{
		int tab_count = 0;

		Indenter& operator++(int)
		{
			tab_count++;

			return *this;
		}

		Indenter& operator--(int)
		{
			tab_count--;

			return *this;
		}

		friend class Archiver& operator<<(class Archiver& archiver, const Indenter& tab);
	};

	class Archiver
	{
		static constexpr int _page_size = 1024*512;

		uint8_t _start[_page_size];
		uint8_t * _end = _start + _page_size;
		uint8_t * _position;

		size_t _size;

		std::string string;

		std::fstream file;

	public:
		Archiver() 
			: _start()
			, _position(&_start[0])
			, _size(0)
		{
			namespace fs = std::filesystem;

			fs::path log_path = std::filesystem::current_path() /= "Logs";

			bool directory_exists = fs::exists(log_path);

			if (!directory_exists)
			{
				fs::create_directory(log_path);
			}

			log_path /= "Log1.html";

			file.open(log_path, std::ios::out | std::ios::trunc);

			if (!file.is_open())
			{
				throw;
			}

			const auto& tab = "    ";

			Indenter indenter;

			*this << "<!DOCTYPE html>"
				<< '\n'
				<< "<html>" << '\n' << '\n'
				<< "<head>" << '\n';

			indenter++;
			*this << indenter << "<title>Log File</title>\n"
				<< indenter << "<style type = \"text/css\">\n";

			indenter++;
			*this
				<< indenter << "#outer {	border-collapse: separate; }\n\n"
				<< indenter << "table, th, td { border: 1px solid black; }\n\n"
				//<< indenter << "table { width: 100%; }\n\n"
				<< indenter << "table.mt { border-collapse: collapse; background-color:lavender; }\n\n"
				<< indenter << "table.mt th { border-collapse: collapse; background-color:green; }\n\n"
				<< indenter << "table.mt th:hover { border-collapse: collapse; background-color:green; }\n\n"
				<< indenter << "table.mt tr { border-collapse: collapse; background-color:thistle; }\n\n"
				<< indenter << "table.mt tr:hover { border-collapse: collapse; background-color:plum; }\n\n"
				<< indenter << "table.mt td:hover { border-collapse: collapse; background-color:darkkhaki; }\n\n"
				<< indenter << "table.mt td.red { border-collapse: collapse; background-color:crimson; }\n\n"
				<< indenter << "table.mt td.red:hover { border-collapse: collapse; background-color:red; }\n\n"
				<< indenter << "table.mt td.green { border-collapse: collapse; background-color:green; }\n\n"
				<< indenter << "table.mt td.green:hover { border-collapse: collapse; background-color:lightgreen; }\n\n";

			indenter--;
			*this << indenter << "</style>\n"
				<< "</head>\n\n"
				<< "<body>\n\n";
		}

		~Archiver()
		{
			_position = nullptr;
			file.close();
		};

		Archiver(const Archiver& other) = delete;

		Archiver(Archiver&& other) = default;

		Archiver& operator=(const Archiver& other) = delete;
				
		Archiver& operator=(Archiver&& other) = default;

		void flush();

		Archiver& operator<<(const char c)
		{
			CopyToBuffer(c);

			return *this;
		}

		Archiver& operator<<(const uint8_t i)
		{
			CopyToBufferFormattedIntger(i, "%lld");

			return *this;
		}

		Archiver& operator<<(const uint16_t i)
		{
			CopyToBufferFormattedIntger(i, "%lld");

			return *this;
		}

		Archiver& operator<<(const uint32_t i)
		{
			CopyToBufferFormattedIntger(i, "%lld");

			return *this;
		}

		Archiver& operator<<(const uint64_t i)
		{
			CopyToBufferFormattedIntger(i, "%lld");

			return *this;
		}

		Archiver& operator<<(const int8_t i)
		{
			CopyToBufferFormattedIntger(i, "%lld");

			return *this;
		}

		Archiver& operator<<(const int16_t i)
		{
			CopyToBufferFormattedIntger(i, "%lld");

			return *this;
		}

		Archiver& operator<<(const int32_t i)
		{
			CopyToBufferFormattedIntger(i, "%lld");

			return *this;
		}

		Archiver& operator<<(const int64_t i)
		{
			CopyToBufferFormattedIntger(i, "%lld");

			return *this;
		}

		Archiver& operator<<(const float f)
		{
			string = std::to_string(f);
			CopyToBuffer(string);

			return *this;
		}

		Archiver& operator<<(const double d)
		{
			string = std::to_string(d);
			CopyToBuffer(string);

			return *this;
		}

		Archiver& operator<<(const long double ld)
		{
			string = std::to_string(ld);
			CopyToBuffer(string);

			return *this;
		}

		Archiver& operator<<(const std::string str)
		{
			CopyToBuffer(str);

			return *this;
		}

		template<std::size_t size_of_string>
		Archiver& operator<<(const char (& string_literal)[size_of_string])
		{
			// used size of string-1 to truncate the /0
			CopyToBuffer(reinterpret_cast<const char*>(&string_literal[0]), size_of_string - 1);
			
			return *this;
		}

		void CopyToBuffer(const char* start, size_t size_in_bytes)
		{
			assert(size_in_bytes < _page_size);

			auto size_remaining = _page_size - _size;

			if (size_in_bytes >= size_remaining)
			{
				flush();
			}

			memcpy_s(_position, size_remaining, start, size_in_bytes);
			_position += static_cast<uint64_t>(size_in_bytes);
			_size += size_in_bytes;
		}

		void CopyToBuffer(std::string string)
		{
			auto size = string.size();

			auto new_size = _size + size;

			if (new_size >= _page_size)
			{
				flush();
				new_size = size;
			}

			memcpy_s(_position, _page_size - _size, string.data(), string.size());
			_position += static_cast<uint64_t>(string.size());
			
			_size = new_size;
		}

		void CopyToBuffer(const char ch)
		{
			if (_size >= _page_size)
			{
				flush();
			}

			memcpy_s(_position, _page_size - _size, &ch, sizeof(char));
			
			_position++;
			_size++;
		}

		template<std::size_t size_of_string>
		void CopyToBufferFormattedIntger(uint64_t integer, const char (& format)[size_of_string])
		{
			// take 1 out to account for \0
			auto buffer_size_remaining = _page_size - _size - 1;

			auto bytes_written = snprintf(reinterpret_cast<char*>(_position), buffer_size_remaining, format, integer);

			if (bytes_written < buffer_size_remaining)
			{
				_position += bytes_written;
			}
			else
			{
				flush();

				bytes_written = snprintf(reinterpret_cast<char*>(_position), _page_size, format, integer);

				if (bytes_written < buffer_size_remaining)
				{
					_position += bytes_written;
				}
				else
				{
					// Probably doesn't fit
					throw;
				}
			}
		}

		void Write(const char ch, int count)
		{
			auto new_size = _size + count;

			if (new_size >= _page_size)
			{
				flush();
				new_size = count;
			}

			for (auto i = _size; i < _size + count; i++)
			{
				_start[i] = ch;
			}

			_position += count;
			_size = new_size;
		}
	};

	inline Archiver& mt::operator<<(Archiver& archiver, const Indenter& tab)
	{
		archiver.Write('\t', tab.tab_count);

		return archiver;
	}
}