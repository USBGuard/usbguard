%include {
}

%name RuleParser
%extra_argument { RuleParserState* state }

%default_destructor { (void)state; }

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
       state->rule.setTarget(Rule::Target::Allow);
}

target ::= KEYWORD_BLOCK. {
       state->rule.setTarget(Rule::Target::Block);
}

target ::= KEYWORD_REJECT. {
       state->rule.setTarget(Rule::Target::Reject);
}

device_spec ::= device_id device_attributes.

device_id ::= HEXCHAR4(V) COLON ASTERISK. { // 1234:*
  	  state->rule.setVendorID(quex::unicode_to_char(V->get_text()));
	  delete V;
}

device_id ::= HEXCHAR4(V) COLON HEXCHAR4(P). { // 1234:5678
	  state->rule.setVendorID(quex::unicode_to_char(V->get_text()));
	  state->rule.setProductID(quex::unicode_to_char(P->get_text()));
	  delete V;
	  delete P;
}

device_id ::= ASTERISK COLON ASTERISK.
device_id ::= .

device_attributes ::= device_attributes device_attribute.
device_attributes ::= .

device_attribute ::= KEYWORD_HASH string(S). {
		 state->rule.setDeviceHash(*S);
		 delete S;
}

device_attribute ::= KEYWORD_NAME string(S). {
		 state->rule.setDeviceName(*S);
		 delete S;
}

device_attribute ::= KEYWORD_SERIAL string(S). {
		 state->rule.setSerialNumber(*S);
		 delete S;
}

device_attribute ::= KEYWORD_VIAPORT string(S). {
		 state->rule.refDevicePorts().push_back(*S);
		 state->rule.setDevicePortsSetOperator(Rule::SetOperator::Equals);
		 delete S;
}

device_attribute ::= KEYWORD_VIAPORT ports_set_op(O) CURLYBRACE_OPEN stringvec(V) CURLYBRACE_CLOSE. {
		 state->rule.refDevicePorts().insert(state->rule.refDevicePorts().end(), V->begin(), V->end());
		 state->rule.setDevicePortsSetOperator(O);
		 delete V;
}

ports_set_op(O) ::= SET_OPERATOR(V). {
		 O = Rule::setOperatorFromString(quex::unicode_to_char(V->get_text()));
		 delete V;
}

ports_set_op(O) ::= . {
		    O = Rule::SetOperator::OneOf;
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
	  state->rule.refInterfaceTypes().push_back(*T);
	  state->rule.setInterfaceTypesSetOperator(Rule::SetOperator::Equals);
	  delete T;
}

device_attribute ::= KEYWORD_WITHINTERFACE usbif_set_op(O) CURLYBRACE_OPEN usbiftypevec(V) CURLYBRACE_CLOSE. {
	  state->rule.refInterfaceTypes().insert(state->rule.refInterfaceTypes().end(), V->begin(), V->end());
	  state->rule.setInterfaceTypesSetOperator(O);
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
       state->rule.setAction(*S);
       delete S;
}

action ::= .

string(S) ::= DQ_STRING_BEGIN DQ_STRING(V) DQ_STRING_END. {
	  const String from_unicode = quex::unicode_to_char(V->get_text());
	  const String unescaped = Rule::unescapeString(from_unicode);
	  S = new String(unescaped);
	  delete V;
}
