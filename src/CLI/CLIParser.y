%include {
#include <cstdint>
}

%name CLIParser
%extra_argument { PointerVector<CLICommand>* commands }

%token_prefix CLI_TOKEN_
%token_type { String* }
%token_destructor { delete $$; }

%type command { CLICommand* }
%destructor command { delete $$; }

%type rule_command { CLICommand* }
%destructor rule_command { delete $$; }

%type device_command { CLICommand* }
%destructor device_command { delete $$; }

%type rule_append_options { RuleAppend::Options* }
%destructor rule_append_options { delete $$; }

%type device_set_policy_options { DeviceSetPolicy::Options* }
%destructor device_set_policy_options { delete $$; }

%type seqn_value { uint32_t }
%type timeout_value { uint32_t }
%type boolean_value { bool }

%type string { std::string* }
%destructor string { delete $$; }

%syntax_error {
    throw std::runtime_error("CLI command syntax error");
}

%nonassoc TERMINATION UNINITIALIZED.

cli ::= commands.
commands ::= commands command(C). {
    commands->push_back(Pointer<CLICommand>(C));
}

commands ::= .
command(L) ::= KEYWORD_RULE rule_command(R). { L = R; }
command(L) ::= KEYWORD_DEVICE device_command(R). { L = R; }

/*
 * usbguard rule append "rule spec" [--parent <seqn>][--timeout <sec>]
 */
rule_command(C) ::= KEYWORD_APPEND string(R) rule_append_options(O). {
    C = new RuleAppend(*R, *O);
    delete O;
}

rule_append_options(O) ::= rule_append_options(P) OPTION_PARENT seqn_value(V). {
    O = P;
    O->parent_seqn = V; 
}

rule_append_options(O) ::= rule_append_options(P) OPTION_TIMEOUT timeout_value(V). {
    O = P;
    O->timeout_sec = V;
}

rule_append_options(O) ::= . {
    O = new RuleAppend::Options();
}


/*
 * usbguard rule remove <seqn>
 */
rule_command ::= KEYWORD_REMOVE rule_remove_args.
rule_remove_args ::= seqn_value.

/*
 * usbguard rule list
 */
rule_command ::= KEYWORD_LIST.

/*
 * usbguard device list
 */
device_command ::= KEYWORD_LIST.

/*
 * usbguard device rule <seqn>
 */
device_command ::= KEYWORD_RULE seqn_value.

/*
 * usbguard device hash <seqn>
 */
device_command ::= KEYWORD_HASH seqn_value.

/*
 * usbguard device set-policy <seqn> <target> [--timeout <sec>][--permanent [<bool>]]
 */
device_command ::= KEYWORD_SET_POLICY seqn_value target_value device_set_policy_options.
device_set_policy_options ::= device_set_policy_options device_set_policy_option.
device_set_policy_options ::= .
device_set_policy_option ::= OPTION_TIMEOUT timeout_value.
device_set_policy_option ::= OPTION_PERMANENT boolean_value.
device_set_policy_option ::= OPTION_PERMANENT.

/*
 * Type values
 */
seqn_value ::= NUMBER_UNSIGNED_INTEGER.
seqn_value ::= KEYWORD_LAST.
seqn_value ::= KEYWORD_ROOT.

timeout_value(V) ::= NUMBER_UNSIGNED_INTEGER(S). {
    V = stringToNumber<uint32_t>(*S, 10);
}

timeout_value(V) ::= NUMBER_HOURS(S). {
    V = stringToNumber<uint32_t>(*S, 10);
    V = V * 60 * 60;
}

timeout_value(V) ::= NUMBER_MINUTES(S). {
    V = stringToNumber<uint32_t>(*S, 10);
    V = V * 60;
}

timeout_value(V) ::= NUMBER_DAYS(S). {
    V = stringToNumber<uint32_t>(*S, 10);
    V = V * 60 * 60 * 24;
}

boolean_value(B) ::= KEYWORD_FALSE. {
    B = false;
}

boolean_value(B) ::= KEYWORD_TRUE. {
    B = true;
}

target_value ::= string.

string(S) ::= STRING(V). {
    S = new std::string(*V);
}

string(S) ::= DQ_STRING_BEGIN STRING(V) DQ_STRING_END. {
    S = new std::string(*V);
}
