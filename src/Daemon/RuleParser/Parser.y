%include {
   #include <build-config.h>

   #include "Common/Logging.hpp"
   #include "Common/Types.hpp"
   #include "Daemon/RuleParser.hpp"
   #include "Lexer.hpp"
   #include <cstddef>

   using namespace usbguard;
}

%name RuleParser
%extra_argument { Firewall::Rule* rule }

%token_prefix RULE_TOKEN_
%token_type { quex::Token* }
%token_destructor { delete $$; }

%type string { String* }
%destructor string { delete $$; }

%type stringvec { StringVector* }
%destructor stringvec { delete $$; }

%syntax_error {
  throw std::runtime_error("syntax error");
}

%nonassoc TERMINATION UNINITIALIZED.

rule ::= rule_spec.

rule_spec ::= target device_spec action.
rule_spec ::= .

target ::= KEYWORD_ALLOW. {
       rule->target = Firewall::Target::Allow;
}

target ::= KEYWORD_DENY. {
       rule->target = Firewall::Target::Deny;
}

target ::= KEYWORD_REJECT. {
       rule->target = Firewall::Target::Reject;
}

device_spec ::= device_id device_attributes.

device_id ::= HEXCHAR4(V) COLON ASTERISK. { // 1234:*
	  rule->id_vendor = (char *)V->get_text().c_str();
	  delete V;
}

device_id ::= HEXCHAR4(V) COLON HEXCHAR4(P). { // 1234:5678
	  rule->id_vendor = (char *)V->get_text().c_str();
	  rule->id_product = (char *)P->get_text().c_str();
	  delete V;
	  delete P;
}

device_id ::= ASTERISK COLON ASTERISK.
device_id ::= .

device_attributes ::= device_attributes device_attribute.
device_attributes ::= .

device_attribute ::= KEYWORD_CLASS string(S). {
		 rule->usb_class = *S;
		 delete S;
}

device_attribute ::= KEYWORD_HASH string(S). {
		 rule->hash = *S;
		 delete S;
}

device_attribute ::= KEYWORD_NAME string(S). {
		 rule->name = *S;
		 delete S;
}

device_attribute ::= KEYWORD_PORT string(S). {
		 rule->id_ports.push_back(*S);
		 delete S;
}

device_attribute ::= KEYWORD_PORT CURLYBRACE_OPEN stringvec(V) CURLYBRACE_CLOSE. {
		 rule->id_ports.insert(rule->id_ports.end(), V->begin(), V->end());
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
       rule->action = *S;
       delete S;
}

action ::= .

string(S) ::= DQ_STRING_BEGIN DQ_STRING(V) DQ_STRING_END. {
	  S = new String((char *)V->get_text().c_str());
	  delete V;
}
