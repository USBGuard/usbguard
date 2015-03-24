%include {
  #include "Typedefs.hpp"
  #include "Rule.hpp"
  #include "Lexer.hpp"
  #include <cstddef>

  using namespace usbguard;
}

%name RuleParser
%extra_argument { Rule* rule }

%token_prefix RULE_TOKEN_
%token_type { quex::Token* }
%token_destructor { delete $$; }

%type string { String* }
%destructor string { delete $$; }

%type stringvec { StringVector* }
%destructor stringvec { delete $$; }

%syntax_error {
  throw std::runtime_error("rule syntax error");
}

%nonassoc TERMINATION UNINITIALIZED.

rule ::= rule_spec.

rule_spec ::= target device_spec action.
rule_spec ::= .

target ::= KEYWORD_ALLOW. {
       rule->setTarget(Rule::Target::Allow);
}

target ::= KEYWORD_BLOCK. {
       rule->setTarget(Rule::Target::Block);
}

target ::= KEYWORD_REJECT. {
       rule->setTarget(Rule::Target::Reject);
}

device_spec ::= device_id device_attributes.

device_id ::= HEXCHAR4(V) COLON ASTERISK. { // 1234:*
  	  rule->setVendorID(quex::unicode_to_char(V->get_text()));
	  delete V;
}

device_id ::= HEXCHAR4(V) COLON HEXCHAR4(P). { // 1234:5678
	  rule->setVendorID(quex::unicode_to_char(V->get_text()));
	  rule->setVendorID(quex::unicode_to_char(P->get_text()));
	  delete V;
	  delete P;
}

device_id ::= ASTERISK COLON ASTERISK.
device_id ::= .

device_attributes ::= device_attributes device_attribute.
device_attributes ::= .

device_attribute ::= KEYWORD_HASH string(S). {
		 rule->setDeviceHash(*S);
		 delete S;
}

device_attribute ::= KEYWORD_NAME string(S). {
		 rule->setDeviceName(*S);
		 delete S;
}

device_attribute ::= KEYWORD_PORT string(S). {
		 rule->refDevicePorts().push_back(*S);
		 delete S;
}

device_attribute ::= KEYWORD_PORT CURLYBRACE_OPEN stringvec(V) CURLYBRACE_CLOSE. {
		 rule->refDevicePorts().insert(rule->refDevicePorts().end(), V->begin(), V->end());
		 delete V;
}

stringvec(D) ::= stringvec(S) string(V). {
	  D = S;
	  D->push_back(*V);
	  delete V;
}

stringvec(V) ::= . {
	  V = new StringVector();
}

action ::= KEYWORD_ACTION string(S). {
       rule->setAction(*S);
       delete S;
}

action ::= .

string(S) ::= DQ_STRING_BEGIN DQ_STRING(V) DQ_STRING_END. {
	  S = new String(quex::unicode_to_char(V->get_text()));
	  delete V;
}
