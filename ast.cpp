#include "include/ast.h"
#include "include/common.h"
#include "include/generator.h"
#include "include/lexer.h"
#include "include/table.h"
#include <sstream>
#include <string>
std::string Node::gen(generator &g)
{
    (void)g;
    if(tok.type==ID) {
        symbol var = search(variant2string(tok.value));
        if(var.comptime) return std::to_string(variant2int<long long>(var.value));
    }
    return variant2value(tok);
}

std::string BinaryNode::gen(generator &g)
{
    bool float_ = false;
    bool string_to_int_op = false;
    if (left->kind == ast_type::JUSTNODE)
    {
        auto n = static_cast<Node *>(left.get());
        auto n_ = static_cast<Node *>(right.get());
        if (is_it_float(n->tok.type))
        {
            float_ = true;
            g.line = n->tok.line;
            g.column = n->tok.column;
        }
        if (is_it_float(n_->tok.type))
        {
            float_ = true;
            g.line = n->tok.line;
            g.column = n->tok.column;
        }
        if (n->tok.type == STRING && (is_it_int(n_->tok.type)))
        {
            string_to_int_op = true;
            g.line = n->tok.line;
            g.column = n->tok.column;
        }
        if (n_->tok.type == STRING && is_it_int(n->tok.type))
        {
            string_to_int_op = true;
            g.line = n->tok.line;
            g.column = n->tok.column;
        }
    }
    std::string res = "(" + g.gencode(left) + op2string(op) + g.gencode(right) + ")";
    if (float_ && op == MOD)
    {
        std::string err = "Cannot do modulo for floats, use fmod from stdlib instead:\n\t";
        err += res + '\n';
        throw TranspileTimeError(err.c_str());
    }
    else if (string_to_int_op)
    {
        std::string err = "Cannot do math with string and integer:\n\t";
        err += res + '\n';
        throw TranspileTimeError(err.c_str());
    }
    return res;
}

std::string AssignmentNode::gen(generator &g)
{
  if(is_const) {
    g.line = id.line;
    g.column = id.column;
    throw TranspileTimeError("Variable '" + variant2string(id.value) + "' is constant\n");
  }
    std::string value = val ? g.gencode(val) : "";
    return " " + variant2string(id.value) + (value.empty() ? "" : "=" + value);
}

std::string AssignmentNodeExpr::gen(generator &g)
{
    std::string type;
    if(is_const) type += "const ";
    std::string nullval = "0";
    if (type_ == BYTE_TYPE)
        type += "int8_t ";
    if (type_ == WORD_TYPE)
        type += "int16_t ";
    if (type_ == INT_TYPE)
        type += "int32_t ";
    if (type_ == LONG_TYPE)
        type += "int64_t ";
    if (type_ == FLOAT_TYPE)
        type += "float ";
    if (type_ == DOUBLE_TYPE)
        type += "double ";
    if (type_ == STRING_TYPE)
    {
        type += "std::string ";
        nullval = "\"\"";
    }
    if (type_ == BOOL_TYPE)
        type += "bool ";
    if (type_ == UNSIGNED_8_TYPE)
        type += "uint8_t ";
    if (type_ == UNSIGNED_16_TYPE)
        type += "uint16_t ";
    if (type_ == UNSIGNED_32_TYPE)
        type += "uint32_t ";
    if (type_ == UNSIGNED_64_TYPE)
        type += "uint64_t ";
    if (type_ == AUTO_TYPE)
        type += "auto ";
    return type + variant2string(id.value) + (val ? "=" + g.gencode(val) : "=" + nullval);
}

std::string UnaryNode::gen(generator &g)
{
    if (sign == MINUS)
        return "-" + g.gencode(left);
    if (sign == NOT)
        return "!" + g.gencode(left);
    return g.gencode(left);
}

std::string FuncCallNode::gen(generator &g)
{
    std::string args_;
    std::string name = variant2string(id.value);
    if (name == "print")
    {
        args_ = "std::cout << ";
        for (auto &x : args)
        {
            args_ += g.gencode(x);
            args_ += " << ";
        }
        args_ += "std::endl";
        return args_;
    }
    if (name == "input")
    {
        args_ = "std::cin ";
        args_ += " >> ";
        args_ += g.gencode(args.at(0));
        return args_;
    }

    if (name == "sizeof")
    {
        args_ = "sizeof(";
        args_ += g.gencode(args.at(0)) + ')';
        return args_;
    }
    for (u64 i = 0; i < args.size(); i++)
    {
        args_ += g.gencode(args[i]);
        if (i + 1 < args.size())
            args_ += ", ";
    }
    return variant2string(id.value) + '(' + args_ + ')';
}

std::string CondNode::gen(generator &g)
{
    std::string op_;
    if (op == LESS)
        op_ = " < ";
    if (op == BIGGER)
        op_ = " > ";
    if (op == LEQUAL)
        op_ = " <= ";
    if (op == BEQUAL)
        op_ = " >= ";
    if (op == EQUAL)
        op_ = " == ";
    if (op == NEQUAL)
        op_ = " != ";
    if (op == OR)
        op_ = " || ";
    if (op == AND)
        op_ = " && ";
    return '(' + g.gencode(left) + op_ + g.gencode(right) + ')';
}

std::string BlockNode::gen(generator &g)
{
    std::ostringstream code;
    code << " {\n";
    g.indent++;
    for (auto &x : stmts)
    {
        code << g.pad();
        code << g.gencode(x);
        code << ";\n";
    }
    g.indent--;
    code << g.pad() + "}";
    return code.str();
}

std::string FuncNode::gen(generator &g)
{
    std::ostringstream code;
    code << type_in_cpp(type);
    code << variant2string(id.value);
    code << '(';
    for (u64 i = 0; i < args.size(); i++)
    {
        code << g.gencode(args[i]);
        if (i + 1 < args.size())
            code << ", ";
    }
    code << ") ";
    code << g.gencode(block);
    code << '\n';
    g.header += code.str();
    return "";
}

std::string ArgumentNode::gen(generator &g)
{
    (void)g;
    std::string type_ = type_in_cpp(type);
    if(is_array) return type_ +  variant2string(id.value) + "[]";
    return type_ + variant2string(id.value);
}

std::string ReturnNode::gen(generator &g)
{
    return "return (" + g.gencode(value) + ')';
}

std::string IfNode::gen(generator &g)
{
    std::string code;
    if (cond)
    {
        code += (type == IF ? "if(" : "else if(") + g.gencode(cond) + ")" + g.gencode(block);
    }
    else
        code += "else " + g.gencode(block);
    if (next)
        code += " " + g.gencode(next);
    return code;
}

std::string LoopNode::gen(generator &g)
{
    std::string code;
    code += "while(";
    code += g.gencode(cond);
    code += ") ";
    code += g.gencode(block);
    return code;
}

std::string BreakNode::gen(generator &g)
{
    (void)g;
    return "break";
}

std::string ContinueNode::gen(generator &g)
{
    (void)g;
    return "continue";
}

std::string IncDecVarNode::gen(generator &g)
{
    (void)g;
    if (type == 0)
    {
        return variant2string(id) + "++";
    }
    else
        return variant2string(id) + "--";
}

std::string ForNode::gen(generator &g)
{
    std::string code = " for(";
    code += g.gencode(var) + ';';
    code += g.gencode(cond) + ';';
    code += g.gencode(thing);
    code += ')';
    code += g.gencode(block);
    return code;
}

std::string ReAssignmentNodeExpr::gen(generator &g)
{
    std::string op;
    switch (type_)
    {
    case PLUS:
    {
        op = " += ";
        break;
    }
    case MINUS:
    {
        op = " -= ";
        break;
    }
    case STAR:
    {
        op = " *= ";
        break;
    }
    case SLASH:
    {
        op = " /= ";
        break;
    }
    case MOD:
    {
        op = " %= ";
        break;
    }
    case XOR:
    {
        op = " ^= ";
        break;
    }
    case OR_B:
    {
        op = " |= ";
        break;
    }
    case AND_B:
    {
        op = " &= ";
        break;
    }
    default:
        break;
    }
    return variant2string(id.value) + op + g.gencode(val);
}

std::string ArrayNode::gen(generator &g)
{
    std::string type_ = type_in_cpp(type);
    std::string values_ = "{";
    if (!values.empty())
    {
        for (u64 i = 0; i < values.size(); i++)
        {
            values_ += g.gencode(values[i]);
            if (i + 1 < values.size())
                values_ += ", ";
        }
        values_ += "}";
    }
    else
    {
        values_ = "";
    }
    if (!values.empty())
        return type_ + id + "[" + std::to_string(size) + "] = " + values_;
    return type_ + id + "[" + std::to_string(size) + "]";
    if (!values.empty())
        return type_ + id + "[] = " + values_;
    return type_ + id + "[]";
}

std::string ArrayAccessNode::gen(generator &g)
{
    return variant2string(id.value) + "[" + g.gencode(index) + "]";
}

std::string ArrayChangeNode::gen(generator &g)
{
    return variant2string(id.value) + "[" + g.gencode(index) + "] = " + g.gencode(value);
}

std::string ModuleNode::gen(generator &g)
{
    std::ostringstream code;
    generator gen;
    code << gen.generate(module);
    g.header += code.str();
    return "";
}
