#include "memoria_utils_unicode.hpp"

#include <memory>

MEMORIA_BEGIN

CUTF8Char::CUTF8Char()
{
	memset(_bytes, 0, kMaxUtf8CharSize);
	_size = 0;
}

CUTF8Char::CUTF8Char(const char *data, size_t length)
{
	if (length > kMaxUtf8CharSize)
	{
		memset(_bytes, 0, kMaxUtf8CharSize);
		_size = 0;
		return;
	}

	memset(&_bytes[length], 0, kMaxUtf8CharSize - length);

	switch (length)
	{
	case 4:
		_bytes[3] = data[3];
		[[fallthrough]];
	case 3:
		_bytes[2] = data[2];
		[[fallthrough]];
	case 2:
		_bytes[1] = data[1];
		[[fallthrough]];
	case 1:
		_bytes[0] = data[0];
		[[fallthrough]];
	default:
		break;
	}

	_size = static_cast<uint8_t>(Utf8SeqLength(_bytes[0]));
}

const char *CUTF8Char::data() const
{
	return _bytes;
}

size_t CUTF8Char::size() const
{
	return _size;
}

bool CUTF8Char::empty() const
{
	return _bytes[0] == 0;
}

void CUTF8Char::clear()
{
	memset(_bytes, 0, kMaxUtf8CharSize);
}

bool CUTF8Char::operator==(const CUTF8Char &other) const
{
	size_t len = size();
	if (len != other.size())
	{
		return false;
	}

	for (size_t i = 0; i < len; ++i)
	{
		if (_bytes[i] != other._bytes[i])
		{
			return false;
		}
	}

	return true;
}

bool CUTF8Char::operator!=(const CUTF8Char &other) const
{
	return !(*this == other);
}

uint32_t CUTF8Char::to_codepoint() const
{
	size_t len = size();

	if (len == 1)
	{
		return static_cast<unsigned char>(_bytes[0]);
	}
	else if (len == 2)
	{
		return ((static_cast<unsigned char>(_bytes[0]) & 0x1F) << 6) |
			(static_cast<unsigned char>(_bytes[1]) & 0x3F);
	}
	else if (len == 3)
	{
		return ((static_cast<unsigned char>(_bytes[0]) & 0x0F) << 12) |
			((static_cast<unsigned char>(_bytes[1]) & 0x3F) << 6) |
			(static_cast<unsigned char>(_bytes[2]) & 0x3F);
	}
	else if (len == 4)
	{
		return ((static_cast<unsigned char>(_bytes[0]) & 0x07) << 18) |
			((static_cast<unsigned char>(_bytes[1]) & 0x3F) << 12) |
			((static_cast<unsigned char>(_bytes[2]) & 0x3F) << 6) |
			(static_cast<unsigned char>(_bytes[3]) & 0x3F);
	}

	return 0;
}

size_t CUTF8Char::write(char *buffer) const
{
	size_t len = size();
	for (size_t i = 0; i < len; ++i)
	{
		buffer[i] = _bytes[i];
	}
	return len;
}

CUChar::CUChar() = default;

CUChar::CUChar(uint32_t value)
	: _value(value)
{
}

uint32_t CUChar::value() const
{
	return _value;
}

void CUChar::set_value(uint32_t value)
{
	_value = value;
}

bool CUChar::empty() const
{
	return _value == 0;
}

void CUChar::clear()
{
	_value = 0;
}

bool CUChar::operator==(const CUChar &other) const
{
	return _value == other._value;
}

bool CUChar::operator!=(const CUChar &other) const
{
	return _value != other._value;
}

bool CUChar::is_valid() const
{
	return IsValidUnicode(_value);
}

size_t CUChar::encode_utf8(char *buffer) const
{
	return EncodeUtf8(_value, buffer);
}

size_t CUChar::encode_utf16(char *buffer) const
{
	return EncodeUtf16(_value, buffer);
}

size_t CUChar::encode_utf32(char *buffer) const
{
	return EncodeUtf32(_value, buffer);
}

size_t CUChar::decode_utf8(const char *buffer)
{
	return DecodeUtf8(buffer, _value);
}

size_t CUChar::decode_utf16(const char *buffer)
{
	return DecodeUtf16(buffer, _value);
}

size_t CUChar::decode_utf32(const char *buffer)
{
	return DecodeUtf32(buffer, _value);
}

CUTF8StringBase::CUTF8StringBase()
	: _buffer(nullptr), _length(0), _capacity(0)
{
}

bool CUTF8StringBase::ensure_capacity(size_t additional_size)
{
	if (_length + additional_size >= _capacity)
	{
		if (!_expandable)
			return false;

		size_t new_capacity = _capacity;
		while (_length + additional_size >= new_capacity)
			new_capacity += 256;

		char *buffer = reinterpret_cast<char *>(realloc(_buffer, new_capacity));
		if (!buffer)
			return false;

		_capacity = new_capacity;
		_buffer = buffer;
	}
	return true;
}

const char *CUTF8StringBase::c_str() const
{
	return _buffer;
}

const char *CUTF8StringBase::data() const
{
	return _buffer;
}

size_t CUTF8StringBase::size() const
{
	return _length;
}

bool CUTF8StringBase::empty() const
{
	return _length == 0;
}

void CUTF8StringBase::clear()
{
	_length = 0;

	if (_buffer)
		memset(_buffer, 0, _capacity);
}

bool CUTF8StringBase::push_back(uint32_t codepoint)
{
	if (_length + kMaxUtf8CharSize >= _capacity)
	{
		if (!_expandable)
			return false;

		char *buffer = reinterpret_cast<char *>(realloc(_buffer, _capacity + 256));
		if (!buffer)
			return false;

		_capacity += 256;
		_buffer = buffer;
	}

	char temp[kMaxUtf8CharSize];
	size_t char_len = EncodeUtf8(codepoint, temp);
	if (char_len == 0)
		return false;

	for (size_t i = 0; i < char_len; ++i)
		_buffer[_length + i] = temp[i];

	_length += char_len;
	_buffer[_length] = '\0';

	return true;
}

bool CUTF8StringBase::pop_back()
{
	if (_length == 0)
		return false;

	size_t i = _length;
	while (i > 0)
	{
		--i;
		unsigned char c = static_cast<unsigned char>(_buffer[i]);
		if ((c & 0xC0) != 0x80)
		{
			_length = i;
			_buffer[_length] = '\0';
			return true;
		}
	}
	return false;
}

bool CUTF8StringBase::from_string(const char *str)
{
	size_t offset = 0;
	while (str[offset] != '\0')
	{
		size_t char_len;
		if (!IsValidUtf8Char(str + offset, static_cast<size_t>(-1), &char_len))
			return false;

		if (!ensure_capacity(char_len))
			return false;

		for (size_t i = 0; i < char_len; ++i)
			_buffer[_length + i] = str[offset + i];

		_length += char_len;
		offset += char_len;
	}

	_buffer[_length] = '\0';
	return true;
}

bool CUTF8StringBase::append(const char *str)
{
	size_t offset = 0;

	while (str[offset] != '\0')
	{
		size_t char_len;
		if (!IsValidUtf8Char(str + offset, static_cast<size_t>(-1), &char_len))
			return false;

		if (!ensure_capacity(char_len))
			return false;

		for (size_t i = 0; i < char_len; ++i)
			_buffer[_length + i] = str[offset + i];

		_length += char_len;
		offset += char_len;
	}

	_buffer[_length] = '\0';
	return true;
}

bool CUTF8StringBase::validate() const
{
	return IsValidUtf8String(_buffer, _length);
}

CUTF8String::CUTF8String()
{
	_buffer = nullptr;
	_length = 0;
	_capacity = 0;
	_expandable = true;
}

CUTF8String::CUTF8String(const char *text)
{
	_buffer = nullptr;
	_length = 0;
	_capacity = 0;
	_expandable = true;

	if (!from_string(text))
		clear();
}

CUTF8String::CUTF8String(const char8_t *text)
{
	_buffer = nullptr;
	_length = 0;
	_capacity = 0;
	_expandable = true;

	if (!from_string(reinterpret_cast<const char *>(text)))
		clear();
}

CUTF8String &CUTF8String::operator=(const char *text)
{
	if (!from_string(text))
		clear();

	return *this;
}

CUTF8String &CUTF8String::operator=(const char8_t *text)
{
	if (!from_string(reinterpret_cast<const char *>(text)))
		clear();

	return *this;
}

static bool IsSurrogatePair(uint32_t value)
{
	return value >= 0xD800 && value <= 0xDFFF;
}

bool IsValidUnicode(uint32_t value)
{
	return value <= 0x10FFFF && !IsSurrogatePair(value);
}

bool IsValidUtf8Char(const char *utf8, size_t size, size_t *length)
{
	if (size == 0)
		return false;

	if (size == static_cast<size_t>(-1))
	{
		unsigned char first = static_cast<unsigned char>(utf8[0]);
		size_t len = Utf8SeqLength(first);
		uint32_t codepoint;
		size_t decoded = DecodeUtf8(utf8, codepoint);

		if (decoded != len || !IsValidUnicode(codepoint))
			return false;

		if (length)
			*length = len;

		return true;
	}

	unsigned char first = static_cast<unsigned char>(utf8[0]);
	size_t len = Utf8SeqLength(first);

	if (len == 0 || len > size)
		return false;

	uint32_t codepoint;
	size_t decoded = DecodeUtf8(utf8, codepoint);

	if (decoded != len || !IsValidUnicode(codepoint))
		return false;

	if (length)
		*length = len;

	return true;
}

bool IsValidUtf16Char(const char *utf16, size_t size, size_t *length)
{
	if (size < 2 && size != static_cast<size_t>(-1))
		return false;

	unsigned char high = static_cast<unsigned char>(utf16[0]);
	unsigned char low = static_cast<unsigned char>(utf16[1]);
	uint16_t word = (high << 8) | low;

	if (word >= 0xD800 && word <= 0xDBFF)
	{
		if (size != static_cast<size_t>(-1) && size < 4)
			return false;

		unsigned char high2 = static_cast<unsigned char>(utf16[2]);
		unsigned char low2 = static_cast<unsigned char>(utf16[3]);
		uint16_t next = (high2 << 8) | low2;

		if (next < 0xDC00 || next > 0xDFFF)
			return false;

		uint32_t codepoint;
		DecodeUtf16(utf16, codepoint);

		if (!IsValidUnicode(codepoint))
			return false;

		if (length)
			*length = 4;

		return true;
	}
	else if (word >= 0xDC00 && word <= 0xDFFF)
	{
		return false;
	}
	else
	{
		uint32_t codepoint;
		DecodeUtf16(utf16, codepoint);

		if (!IsValidUnicode(codepoint))
			return false;

		if (length)
			*length = 2;

		return true;
	}
}

bool IsValidUtf32Char(const char *utf32, size_t size, size_t *length)
{
	if (size != static_cast<size_t>(-1) && size < 4)
		return false;

	uint32_t codepoint;
	DecodeUtf32(utf32, codepoint);

	if (!IsValidUnicode(codepoint))
		return false;

	if (length)
		*length = 4;

	return true;
}

bool IsValidUtf8String(const char *utf8, size_t size)
{
	size_t offset = 0;
	if (size == static_cast<size_t>(-1))
	{
		while (utf8[offset] != '\0')
		{
			size_t len;
			if (!IsValidUtf8Char(utf8 + offset, static_cast<size_t>(-1), &len))
				return false;
			offset += len;
		}
		return true;
	}

	while (offset < size)
	{
		size_t len;
		if (!IsValidUtf8Char(utf8 + offset, size - offset, &len))
			return false;
		offset += len;
	}
	return true;
}

bool IsValidUtf16String(const char *utf16, size_t size)
{
	size_t offset = 0;
	if (size == static_cast<size_t>(-1))
	{
		while (true)
		{
			if (utf16[offset] == 0 && utf16[offset + 1] == 0)
				return true;

			size_t len;
			if (!IsValidUtf16Char(utf16 + offset, static_cast<size_t>(-1), &len))
				return false;
			offset += len;
		}
	}

	while (offset < size)
	{
		size_t len;
		if (!IsValidUtf16Char(utf16 + offset, size - offset, &len))
			return false;
		offset += len;
	}
	return true;
}

bool IsValidUtf32String(const char *utf32, size_t size)
{
	size_t offset = 0;
	if (size == static_cast<size_t>(-1))
	{
		while (true)
		{
			if (utf32[offset] == 0 && utf32[offset + 1] == 0 &&
				utf32[offset + 2] == 0 && utf32[offset + 3] == 0)
				return true;

			size_t len;
			if (!IsValidUtf32Char(utf32 + offset, static_cast<size_t>(-1), &len))
				return false;
			offset += len;
		}
	}

	while (offset < size)
	{
		size_t len;
		if (!IsValidUtf32Char(utf32 + offset, size - offset, &len))
			return false;
		offset += len;
	}
	return true;
}

uint32_t Utf8ToCodepoint(const uint8_t *bytes, size_t len)
{
	if (len == 1)
	{
		return static_cast<unsigned char>(bytes[0]);
	}
	else if (len == 2)
	{
		return ((static_cast<unsigned char>(bytes[0]) & 0x1F) << 6) |
			(static_cast<unsigned char>(bytes[1]) & 0x3F);
	}
	else if (len == 3)
	{
		return ((static_cast<unsigned char>(bytes[0]) & 0x0F) << 12) |
			((static_cast<unsigned char>(bytes[1]) & 0x3F) << 6) |
			(static_cast<unsigned char>(bytes[2]) & 0x3F);
	}
	else if (len == 4)
	{
		return ((static_cast<unsigned char>(bytes[0]) & 0x07) << 18) |
			((static_cast<unsigned char>(bytes[1]) & 0x3F) << 12) |
			((static_cast<unsigned char>(bytes[2]) & 0x3F) << 6) |
			(static_cast<unsigned char>(bytes[3]) & 0x3F);
	}

	return 0;
}

size_t Utf8SeqLength(unsigned char first)
{
	if ((first & 0x80) == 0x00) return 1;
	if ((first & 0xE0) == 0xC0) return 2;
	if ((first & 0xF0) == 0xE0) return 3;
	if ((first & 0xF8) == 0xF0) return 4;
	return 0;
}

bool Utf8Validate(const char *data, size_t size)
{
	if (size == 0 || data == nullptr)
	{
		return false;
	}

	unsigned char first = static_cast<unsigned char>(data[0]);
	size_t len = Utf8SeqLength(first);

	if (len == 0 || len > size)
	{
		return false;
	}

	for (size_t i = 1; i < len; ++i)
	{
		if ((static_cast<unsigned char>(data[i]) & 0xC0) != 0x80)
		{
			return false;
		}
	}

	return true;
}

size_t EncodeUtf8(uint32_t value, char *buffer)
{
	if (value <= 0x7F)
	{
		buffer[0] = static_cast<char>(value);
		return 1;
	}

	if (value <= 0x7FF)
	{
		buffer[0] = static_cast<char>(0xC0 | (value >> 6));
		buffer[1] = static_cast<char>(0x80 | (value & 0x3F));
		return 2;
	}

	if (value <= 0xFFFF)
	{
		buffer[0] = static_cast<char>(0xE0 | (value >> 12));
		buffer[1] = static_cast<char>(0x80 | ((value >> 6) & 0x3F));
		buffer[2] = static_cast<char>(0x80 | (value & 0x3F));
		return 3;
	}

	if (value <= 0x10FFFF)
	{
		buffer[0] = static_cast<char>(0xF0 | (value >> 18));
		buffer[1] = static_cast<char>(0x80 | ((value >> 12) & 0x3F));
		buffer[2] = static_cast<char>(0x80 | ((value >> 6) & 0x3F));
		buffer[3] = static_cast<char>(0x80 | (value & 0x3F));
		return 4;
	}

	return 0;
}

size_t EncodeUtf16(uint32_t value, char *buffer)
{
	if (value <= 0xFFFF)
	{
		buffer[0] = static_cast<char>(value >> 8);
		buffer[1] = static_cast<char>(value & 0xFF);
		return 2;
	}

	uint32_t code_point = value - 0x10000;

	buffer[0] = static_cast<char>(0xD8 | (code_point >> 10));
	buffer[1] = static_cast<char>((code_point >> 2) & 0xFF);
	buffer[2] = static_cast<char>(0xDC | (code_point & 0x3FF));
	buffer[3] = static_cast<char>((code_point >> 8) & 0xFF);

	return 4;
}

size_t EncodeUtf32(uint32_t value, char *buffer)
{
	buffer[0] = static_cast<char>(value >> 24);
	buffer[1] = static_cast<char>((value >> 16) & 0xFF);
	buffer[2] = static_cast<char>((value >> 8) & 0xFF);
	buffer[3] = static_cast<char>(value & 0xFF);
	return 4;
}

size_t DecodeUtf8(const char *buffer, uint32_t &value)
{
	unsigned char c = static_cast<unsigned char>(buffer[0]);

	if (c <= 0x7F)
	{
		value = c;
		return 1;
	}
	if ((c & 0xE0) == 0xC0)
	{
		value = (c & 0x1F) << 6;
		value |= (static_cast<unsigned char>(buffer[1]) & 0x3F);
		return 2;
	}
	if ((c & 0xF0) == 0xE0)
	{
		value = (c & 0x0F) << 12;
		value |= (static_cast<unsigned char>(buffer[1]) & 0x3F) << 6;
		value |= (static_cast<unsigned char>(buffer[2]) & 0x3F);
		return 3;
	}
	if ((c & 0xF8) == 0xF0)
	{
		value = (c & 0x07) << 18;
		value |= (static_cast<unsigned char>(buffer[1]) & 0x3F) << 12;
		value |= (static_cast<unsigned char>(buffer[2]) & 0x3F) << 6;
		value |= (static_cast<unsigned char>(buffer[3]) & 0x3F);
		return 4;
	}
	return 0;
}

size_t DecodeUtf16(const char *buffer, uint32_t &value)
{
	unsigned char high = static_cast<unsigned char>(buffer[0]);
	unsigned char low = static_cast<unsigned char>(buffer[1]);
	if ((high & 0xD8) == 0xD8)
	{
		uint32_t high_surrogate = (high & 0x3F) << 8 | low;
		unsigned char high2 = static_cast<unsigned char>(buffer[2]);
		unsigned char low2 = static_cast<unsigned char>(buffer[3]);
		uint32_t low_surrogate = (high2 & 0x3F) << 8 | low2;
		value = ((high_surrogate - 0xD800) << 10) + (low_surrogate - 0xDC00) + 0x10000;
		return 4;
	}
	value = (high << 8) | low;
	return 2;
}

size_t DecodeUtf32(const char *buffer, uint32_t &value)
{
	value = (static_cast<unsigned char>(buffer[0]) << 24) |
		(static_cast<unsigned char>(buffer[1]) << 16) |
		(static_cast<unsigned char>(buffer[2]) << 8) |
		static_cast<unsigned char>(buffer[3]);

	return 4;
}

MEMORIA_END