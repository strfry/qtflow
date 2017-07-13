%{
#include <cstdlib>
#include "yysettings.h"
#include <iostream>

#include <QString>

int settingslex();
void settingserror(const char *s);
%}

%option header-file="lexsettings.h"
%option outfile="lexsettings.cpp"
%option noyywrap
%option nounput
%option prefix="settings"

%%

#!.*        { /* shebang */ }
#.*         { /* comment */ }

=           { return ASSIGN; }
set         { return SET;    }
\r\n        { return ENDL;   }
\n          { return ENDL;   }

[ \t]                    { settingslval.v_char = yytext[0]; return SPACE; }
[[:alnum:]]              { settingslval.v_char = yytext[0]; return ALPHANUM; }
[[:cntrl:][:punct:]]     { settingslval.v_char = yytext[0]; return SPECIAL; }

.           { settingserror(yytext); }

%%