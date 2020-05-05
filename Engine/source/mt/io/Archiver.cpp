#include "io/Archiver.hpp"

using namespace mt::io;

void Archiver::flush()
{
	file.write(reinterpret_cast<const char*>(_start), _size);
	_position = _start;
	_size = 0;
}
