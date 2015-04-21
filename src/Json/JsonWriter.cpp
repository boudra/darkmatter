#include "JsonWriter.hpp"

namespace dm {

	void JsonWriter::PrintNode(JsonNode& node)
	{

		if (node.GetType() == JsonNode::Type::Value)
		{
			const Any& value = node.GetValue();

			if (value.Type().is<long>())
			{
				m_buffer << value.Cast<long>();
			}
			else if (value.Type().is<int>())
			{
				m_buffer << value.Cast<int>();
			}
			else if (value.Type().is<double>())
			{
				m_buffer << value.Cast<double>();
			}
			else if (value.Type().is<std::string>())
			{
				m_buffer << "\"" << value.Cast<std::string>() << "\"";
			}
			else if (value.Type().is<bool>())
			{
				m_buffer << (value.Cast<bool>() ? "true" : "false");
			}
			else if (value.Type().is<std::nullptr_t>())
			{
				m_buffer << "null";
			}

		}
		else
		{
			char brackets = node.IsArray() ? '[' : '{';
			m_buffer << brackets;

			if (node.ChildCount() > 0)
			{
				auto children = node.Children();
				auto last_child = children.end();
				last_child--;

				m_buffer << '\n';

				m_printlevel++;

				for (auto i = children.begin(); i != children.end(); ++i)
				{
					m_buffer << std::string(m_printlevel * m_identWidth, ' ');

					if (node.IsObject())
					{
						m_buffer << "\"" << i->first << "\" : ";
					}

					PrintNode(i->second);
					if (i != last_child) m_buffer << ',';

					m_buffer << '\n';

				}

				m_printlevel--;

				m_buffer << std::string(m_printlevel * m_identWidth, ' ');
			}


			m_buffer << char(brackets + 2);

		}

	}

}
