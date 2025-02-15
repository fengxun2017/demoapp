
<%
    structs = schema['struct_def']
%>
// Auto-generated code for message: ${schema['name']}
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>

// Basic type parsers
uint16_t parse_uint16(const std::vector<uint8_t>& data, size_t& offset) {
    uint16_t value = (data[offset] << 8) | data[offset+1];
    offset += 2;
    return value;
}

uint32_t parse_uint32(const std::vector<uint8_t>& data, size_t& offset) {
    uint32_t value = (data[offset] << 24) | (data[offset+1] << 16) | (data[offset+2] << 8) | data[offset+3];
    offset += 4;
    return value;
}

// Forward declarations
% for struct in structs:
struct ${struct['name']};
std::string display_${struct['name']}(const ${struct['name']}& obj);
% endfor

% for struct in structs:
// Structure for ${struct['name']}
struct ${struct['name']} {
    % for field in struct['desc']:
    % if field['type'] == 'array':
    ${'int8_t' if field['item_type'] == 'int8' else 'uint8_t'} ${field['name']}[${field['len']}];
    % else:
    ${'uint32_t' if field['type'] == 'uint32' else 'uint16_t'} ${field['name']};
    % endif
    % endfor
};

${struct['name']} parse_${struct['name']}(const std::vector<uint8_t>& data, size_t& offset) {
    ${struct['name']} obj;
    % for field in struct['desc']:
    % if field['type'] == 'array':
    for (int i = 0; i < ${field['len']}; ++i) {
        obj.${field['name']}[i] = data[offset++];
    }
    % elif field['type'] == 'uint32':
    obj.${field['name']} = parse_uint32(data, offset);
    % elif field['type'] == 'uint16':
    obj.${field['name']} = parse_uint16(data, offset);
    % endif
    % endfor
    return obj;
}

std::string display_${struct['name']}(const ${struct['name']}& obj) {
    std::stringstream ss;
    % for field in struct['desc']:
    % if field['type'] == 'array':
    ss << "${field['name']}: " << std::string(obj.${field['name']}, obj.${field['name']} + ${field['len']}) << "\\n";
    % else:
    ss << "${field['name']}: " << obj.${field['name']} << "\\n";
    % endif
    % endfor
    return ss.str();
}

% endfor

// Main message parser
struct ${schema['name']} {
    % for field in schema['main']:
    % if field['type'] == 'struct':
    ${field['item_type']} ${field['name']};
    % elif field['type'] == 'struct_array':
    std::vector<${field['item_type']}> ${field['name']};
    % else:
    ${field['type']} ${field['name']};
    % endif
    % endfor
};

${schema['name']} parse_${schema['name']}(const std::vector<uint8_t>& data) {
    size_t offset = 0;
    ${schema['name']} msg;
    % for field in schema['main']:
    % if field['type'] == 'struct':
    msg.${field['name']} = parse_${field['item_type']}(data, offset);
    % elif field['type'] == 'struct_array':
    uint16_t ${field['len_name']} = parse_uint16(data, offset);
    for (int i = 0; i < ${field['len_name']}; ++i) {
        msg.${field['name']}.push_back(parse_${field['item_type']}(data, offset));
    }
    % else:
    msg.${field['name']} = parse_${field['type']}(data, offset);
    % endif
    % endfor
    return msg;
}

std::string display_${schema['name']}(const ${schema['name']}& msg) {
    std::stringstream ss;
    ss << "=== ${schema['name']} ===\\n";
    % for field in schema['main']:
    % if field['type'] == 'struct':
    ss << "${field['name']}:\\n" << display_${field['item_type']}(msg.${field['name']});
    % elif field['type'] == 'struct_array':
    ss << "${field['name']} (count: " << msg.${field['name']}.size() << "):\\n";
    for (const auto& item : msg.${field['name']}) {
        ss << display_${field['item_type']}(item);
    }
    % else:
    ss << "${field['name']}: " << msg.${field['name']} << "\\n";
    % endif
    % endfor
    return ss.str();
}


def generate_cpp_code(schema_data):
    return cpp_template.render(schema=schema_data)

# 使用示例
if __name__ == "__main__":
    with open("your_schema.json") as f:
        full_data = json.load(f)
    
    for msg_id, msg_data in full_data.items():
        if msg_data["needToParse"] == 1:
            code = generate_cpp_code(msg_data["schema"])
            with open(f"{msg_data['schema']['name']}_parser.cpp", "w") as f:
                f.write(code)