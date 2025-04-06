#include "memoria_core_signature.hpp"

#include <assert.h>

MEMORIA_BEGIN

static int HexToInt(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';

	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;

	if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;

	return 0;
}

CSignature::CSignature(const std::string_view &str)
	: _payload{}, _mask{}, _has_optionals(false)
{
	size_t i = 0;

	while (i < str.size())
	{
		while (i < str.size() && str[i] == ' ')
			i++;

		if (i >= str.size())
		{
			return;
		}
		else if (str[i] == '?')
		{
			_has_optionals = true;

			_payload.push_back('\x00');
			_mask.push_back('?');

			while (i < str.size() && str[i] == '?')
				i++;
		}
		else
		{
			auto nibble_l = HexToInt(str[i]); i++;
			auto nibble_r = HexToInt(str[i]); i++;

			_payload.push_back((nibble_l << 4) | (nibble_r));
			_mask.push_back('x');
		}
	}
}

CSignature::CSignature(const void *data, size_t size, std::optional<uint8_t> ignore_byte)
	: _payload{}, _mask{}, _has_optionals(false)
{
	_payload.reserve(size);
	_mask.reserve(size);

	const uint8_t *bytes = static_cast<const uint8_t *>(data);

	for (size_t i = 0; i < size; ++i)
	{
		if (ignore_byte && bytes[i] == *ignore_byte)
		{
			_has_optionals = true;
			_payload.push_back(0x00);
			_mask.push_back('?');
		}
		else
		{
			_payload.push_back(bytes[i]);
			_mask.push_back('x');
		}
	}
}

std::vector<std::optional<uint8_t>> CSignature::CreatePattern() const
{
	std::vector<std::optional<uint8_t>> std_sig;
	std_sig.reserve(_payload.size());

	for (size_t i = 0; i < _payload.size(); i++)
	{
		if (_mask[i] == 'x')
			std_sig.push_back(_payload[i]);
		else
			std_sig.push_back(std::nullopt);
	}

	return std_sig;
}

bool CSignature::Match(const void *addr) const
{
	const uint8_t *data = static_cast<const uint8_t *>(addr);

	if (!_has_optionals)
	{
		return std::memcmp(data, _payload.data(), _payload.size()) == 0;
	}
	else
	{
		for (size_t i = 0; i < _payload.size(); i++)
		{
			if (_mask[i] == 'x' && data[i] != _payload[i])
			{
				return false;
			}
		}

		return true;
	}
}

bool CSignature::IsEmpty() const
{
	assert(_payload.empty() == _mask.empty());

	return _payload.empty();
}

bool CSignature::HasOptionals() const
{
	return _has_optionals;
}

MEMORIA_END