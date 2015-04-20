%include {
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

%type usbiftype { USBInterfaceType* }
%destructor usbiftype { delete $$; }

%type usbiftypevec { std::vector<USBInterfaceType>* }
%destructor usbiftypevec { delete $$; }

%type ports_set_op { Rule::SetOperator }
%type usbif_set_op { Rule::SetOperator }

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
	  rule->setProductID(quex::unicode_to_char(P->get_text()));
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

device_attribute ::= KEYWORD_SERIAL string(S). {
		 rule->setSerialNumber(*S);
		 delete S;
}

device_attribute ::= KEYWORD_VIAPORT string(S). {
		 rule->refDevicePorts().push_back(*S);
		 delete S;
}

device_attribute ::= KEYWORD_VIAPORT ports_set_op(O) CURLYBRACE_OPEN stringvec(V) CURLYBRACE_CLOSE. {
		 rule->refDevicePorts().insert(rule->refDevicePorts().end(), V->begin(), V->end());
		 rule->setDevicePortsSetOperator(O);
		 delete V;
}

ports_set_op(O) ::= SET_OPERATOR(V). {
		    O = Rule::setOperatorFromString(quex::unicode_to_char(V->get_text()));
		    delete V;
}

ports_set_op(O) ::= . {
		    O = Rule::SetOperator::EqualsOrdered;
}

stringvec(D) ::= stringvec(S) string(V). {
	  D = S;
	  D->push_back(*V);
	  delete V;
}

stringvec(V) ::= . {
	  V = new StringVector();
}

device_attribute ::= KEYWORD_WITHINTERFACE usbiftype(T). {
	  rule->refInterfaceTypes().push_back(*T);
	  delete T;
}

device_attribute ::= KEYWORD_WITHINTERFACE usbif_set_op(O) CURLYBRACE_OPEN usbiftypevec(V) CURLYBRACE_CLOSE. {
	  rule->refInterfaceTypes().insert(rule->refInterfaceTypes().end(), V->begin(), V->end());
	  rule->setInterfaceTypesSetOperator(O);
	  delete V;
}

usbif_set_op(O) ::= SET_OPERATOR(V). {
		    O = Rule::setOperatorFromString(quex::unicode_to_char(V->get_text()));
		    delete V;
}

usbif_set_op(O) ::= . {
		    O = Rule::SetOperator::EqualsOrdered;
}

usbiftype(T) ::= HEXCHAR2(C) COLON HEXCHAR2(S) COLON HEXCHAR2(P). {
	     T = new USBInterfaceType(stringToNumber<uint8_t>(quex::unicode_to_char(C->get_text()), 16),
				      stringToNumber<uint8_t>(quex::unicode_to_char(S->get_text()), 16),
				      stringToNumber<uint8_t>(quex::unicode_to_char(P->get_text()), 16));
	     delete C;
	     delete S;
	     delete P;
}

usbiftype(T) ::= HEXCHAR2(C) COLON HEXCHAR2(S) COLON ASTERISK. {
	     T = new USBInterfaceType(stringToNumber<uint8_t>(quex::unicode_to_char(C->get_text()), 16),
				      stringToNumber<uint8_t>(quex::unicode_to_char(S->get_text()), 16),
				      0,
				      USBInterfaceType::MatchClass|USBInterfaceType::MatchSubClass),
	     delete C;
	     delete S;
}

usbiftype(T) ::= HEXCHAR2(C) COLON ASTERISK COLON ASTERISK. {
	     T = new USBInterfaceType(stringToNumber<uint8_t>(quex::unicode_to_char(C->get_text()), 16),
				      0,
				      0,
				      USBInterfaceType::MatchClass);
	     delete C;
}

usbiftypevec(D) ::= usbiftypevec(S) usbiftype(V). {
	D = S;
	D->push_back(*V);
	delete V;
}

usbiftypevec(V) ::= . {
	V = new std::vector<USBInterfaceType>();
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
