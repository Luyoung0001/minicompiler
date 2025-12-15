/*
 * lexer.c - 词法分析器实现
 * Step 2: 词法分析器
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

/* 关键字表 */
static struct {
    const char *name;
    TokenType type;
} keywords[] = {
    {"int",    TOK_INT},
    {"char",   TOK_CHAR},
    {"void",   TOK_VOID},
    {"return", TOK_RETURN},
    {"if",     TOK_IF},
    {"else",   TOK_ELSE},
    {"while",  TOK_WHILE},
    {"for",    TOK_FOR},
    {NULL,     TOK_EOF}
};

/* Token 类型名称 */
const char *token_type_name(TokenType type) {
    static const char *names[] = {
        [TOK_EOF]       = "EOF",
        [TOK_ERROR]     = "ERROR",
        [TOK_INT]       = "INT",
        [TOK_CHAR]      = "CHAR",
        [TOK_VOID]      = "VOID",
        [TOK_RETURN]    = "RETURN",
        [TOK_IF]        = "IF",
        [TOK_ELSE]      = "ELSE",
        [TOK_WHILE]     = "WHILE",
        [TOK_FOR]       = "FOR",
        [TOK_NUM]       = "NUM",
        [TOK_STRING]    = "STRING",
        [TOK_IDENT]     = "IDENT",
        [TOK_PLUS]      = "PLUS",
        [TOK_MINUS]     = "MINUS",
        [TOK_STAR]      = "STAR",
        [TOK_SLASH]     = "SLASH",
        [TOK_PERCENT]   = "PERCENT",
        [TOK_ASSIGN]    = "ASSIGN",
        [TOK_EQ]        = "EQ",
        [TOK_NE]        = "NE",
        [TOK_LT]        = "LT",
        [TOK_LE]        = "LE",
        [TOK_GT]        = "GT",
        [TOK_GE]        = "GE",
        [TOK_AND]       = "AND",
        [TOK_OR]        = "OR",
        [TOK_NOT]       = "NOT",
        [TOK_AMP]       = "AMP",
        [TOK_PIPE]      = "PIPE",
        [TOK_LPAREN]    = "LPAREN",
        [TOK_RPAREN]    = "RPAREN",
        [TOK_LBRACE]    = "LBRACE",
        [TOK_RBRACE]    = "RBRACE",
        [TOK_LBRACKET]  = "LBRACKET",
        [TOK_RBRACKET]  = "RBRACKET",
        [TOK_COMMA]     = "COMMA",
        [TOK_SEMICOLON] = "SEMICOLON",
    };
    return names[type];
}

/* 初始化词法分析器 */
void lexer_init(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->pos = source;
    lexer->line = 1;
    lexer->col = 1;
    lexer->current.type = TOK_EOF;
    lexer->current.value = NULL;
}

/* 获取当前字符 */
static char peek(Lexer *lexer) {
    return *lexer->pos;
}

/* 获取下一个字符 */
static char peek_next(Lexer *lexer) {
    if (*lexer->pos == '\0') return '\0';
    return *(lexer->pos + 1);
}

/* 前进一个字符 */
static char advance(Lexer *lexer) {
    char c = *lexer->pos++;
    if (c == '\n') {
        lexer->line++;
        lexer->col = 1;
    } else {
        lexer->col++;
    }
    return c;
}

/* 跳过空白字符和注释 */
static void skip_whitespace(Lexer *lexer) {
    while (1) {
        char c = peek(lexer);
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance(lexer);
        } else if (c == '/' && peek_next(lexer) == '/') {
            /* 单行注释 */
            while (peek(lexer) != '\n' && peek(lexer) != '\0') {
                advance(lexer);
            }
        } else if (c == '/' && peek_next(lexer) == '*') {
            /* 多行注释 */
            advance(lexer); /* / */
            advance(lexer); /* * */
            while (!(peek(lexer) == '*' && peek_next(lexer) == '/')) {
                if (peek(lexer) == '\0') break;
                advance(lexer);
            }
            if (peek(lexer) != '\0') {
                advance(lexer); /* * */
                advance(lexer); /* / */
            }
        } else {
            break;
        }
    }
}

/* 创建 token */
static Token make_token(Lexer *lexer, TokenType type, const char *start, int len) {
    Token tok;
    tok.type = type;
    tok.line = lexer->line;
    tok.col = lexer->col - len;
    if (len > 0) {
        tok.value = malloc(len + 1);
        strncpy(tok.value, start, len);
        tok.value[len] = '\0';
    } else {
        tok.value = NULL;
    }
    return tok;
}

/* 创建简单 token (无值) */
static Token make_simple_token(Lexer *lexer, TokenType type) {
    Token tok;
    tok.type = type;
    tok.line = lexer->line;
    tok.col = lexer->col;
    tok.value = NULL;
    return tok;
}

/* 创建错误 token */
static Token make_error(Lexer *lexer, const char *msg) {
    Token tok;
    tok.type = TOK_ERROR;
    tok.line = lexer->line;
    tok.col = lexer->col;
    tok.value = strdup(msg);
    return tok;
}

/* 检查是否是关键字 */
static TokenType check_keyword(const char *str, int len) {
    for (int i = 0; keywords[i].name; i++) {
        if (strlen(keywords[i].name) == (size_t)len &&
            strncmp(keywords[i].name, str, len) == 0) {
            return keywords[i].type;
        }
    }
    return TOK_IDENT;
}

/* 扫描标识符或关键字 */
static Token scan_identifier(Lexer *lexer) {
    const char *start = lexer->pos;
    while (isalnum(peek(lexer)) || peek(lexer) == '_') {
        advance(lexer);
    }
    int len = lexer->pos - start;
    TokenType type = check_keyword(start, len);
    return make_token(lexer, type, start, len);
}

/* 扫描数字 */
static Token scan_number(Lexer *lexer) {
    const char *start = lexer->pos;
    while (isdigit(peek(lexer))) {
        advance(lexer);
    }
    int len = lexer->pos - start;
    return make_token(lexer, TOK_NUM, start, len);
}

/* 扫描字符串 */
static Token scan_string(Lexer *lexer) {
    advance(lexer); /* 跳过开头的引号 */
    const char *start = lexer->pos;
    while (peek(lexer) != '"' && peek(lexer) != '\0') {
        if (peek(lexer) == '\\') {
            advance(lexer); /* 跳过转义字符 */
        }
        advance(lexer);
    }
    int len = lexer->pos - start;
    if (peek(lexer) == '"') {
        advance(lexer); /* 跳过结尾的引号 */
    }
    return make_token(lexer, TOK_STRING, start, len);
}

/* 获取下一个 token */
Token lexer_next(Lexer *lexer) {
    skip_whitespace(lexer);

    if (peek(lexer) == '\0') {
        return make_simple_token(lexer, TOK_EOF);
    }

    char c = peek(lexer);

    /* 标识符或关键字 */
    if (isalpha(c) || c == '_') {
        return scan_identifier(lexer);
    }

    /* 数字 */
    if (isdigit(c)) {
        return scan_number(lexer);
    }

    /* 字符串 */
    if (c == '"') {
        return scan_string(lexer);
    }

    /* 运算符和分隔符 */
    advance(lexer);
    switch (c) {
        case '+': return make_simple_token(lexer, TOK_PLUS);
        case '-': return make_simple_token(lexer, TOK_MINUS);
        case '*': return make_simple_token(lexer, TOK_STAR);
        case '/': return make_simple_token(lexer, TOK_SLASH);
        case '%': return make_simple_token(lexer, TOK_PERCENT);
        case '(':  return make_simple_token(lexer, TOK_LPAREN);
        case ')': return make_simple_token(lexer, TOK_RPAREN);
        case '{': return make_simple_token(lexer, TOK_LBRACE);
        case '}': return make_simple_token(lexer, TOK_RBRACE);
        case '[': return make_simple_token(lexer, TOK_LBRACKET);
        case ']': return make_simple_token(lexer, TOK_RBRACKET);
        case ',': return make_simple_token(lexer, TOK_COMMA);
        case ';': return make_simple_token(lexer, TOK_SEMICOLON);
        case '=':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_simple_token(lexer, TOK_EQ);
            }
            return make_simple_token(lexer, TOK_ASSIGN);
        case '!':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_simple_token(lexer, TOK_NE);
            }
            return make_simple_token(lexer, TOK_NOT);
        case '<':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_simple_token(lexer, TOK_LE);
            }
            return make_simple_token(lexer, TOK_LT);
        case '>':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_simple_token(lexer, TOK_GE);
            }
            return make_simple_token(lexer, TOK_GT);
        case '&':
            if (peek(lexer) == '&') {
                advance(lexer);
                return make_simple_token(lexer, TOK_AND);
            }
            return make_simple_token(lexer, TOK_AMP);
        case '|':
            if (peek(lexer) == '|') {
                advance(lexer);
                return make_simple_token(lexer, TOK_OR);
            }
            return make_simple_token(lexer, TOK_PIPE);
    }

    return make_error(lexer, "unexpected character");
}

/* 查看当前 token */
Token lexer_peek(Lexer *lexer) {
    if (lexer->current.type == TOK_EOF && lexer->current.value == NULL) {
        lexer->current = lexer_next(lexer);
    }
    return lexer->current;
}

/* 释放 token 资源 */
void token_free(Token *token) {
    if (token->value) {
        free(token->value);
        token->value = NULL;
    }
}
