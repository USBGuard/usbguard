//
// Copyright (C) 2016 Red Hat, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Daniel Kopecek <dkopecek@redhat.com>
//
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

%type condition { RuleCondition* }
%destructor condition { delete $$; }

%type conditionvec { std::vector<RuleCondition*>* }
%destructor conditionvec { delete $$; }

%type negation_op { bool }
%type ports_set_op { Rule::SetOperator }
%type usbif_set_op { Rule::SetOperator }
%type condition_set_op { Rule::SetOperator }

%syntax_error {
#ifndef NDEBUG
  std::string possible_tokens;
  const size_t n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
  for (size_t i = 0; i < n; ++i) {
    const int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
    if (a < YYNSTATE + YYNRULE) {
      if (!possible_tokens.empty()) {
        possible_tokens.append(", ");
      }
      possible_tokens.append(yyTokenName[i]);
    }
  }
  const std::string hint_prefix = "Syntax error!";
  if (!possible_tokens.empty()) {
    state->error.setHint(hint_prefix + " Possible tokens: " + possible_tokens);
  }
  else {
    state->error.setHint(hint_prefix);
  }
#else
  state->error.setHint("Syntax error!");
#endif
  throw state->error;
}

%nonassoc TERMINATION UNINITIALIZED.

rule ::= rule_spec.

rule_spec ::= target device_id device_attributes conditions.
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

target ::= KEYWORD_MATCH. {
  state->rule.setTarget(Rule::Target::Match);
}

target ::= KEYWORD_DEVICE. {
  state->rule.setTarget(Rule::Target::Device);
}

device_id ::= DEVICE_VID(V). { // 1234:*
  StringVector tokens;
  tokenizeString(quex::unicode_to_char(V->get_text()), tokens, ":", /*trim_empty=*/false);
  state->rule.setVendorID(tokens[0]);
  delete V;
}

device_id ::= DEVICE_VPID(V). { // 1234:5678
  StringVector tokens;
  tokenizeString(quex::unicode_to_char(V->get_text()), tokens, ":", /*trim_empty=*/false);
  state->rule.setVendorID(tokens[0]);
  state->rule.setProductID(tokens[1]);
  delete V;
}

device_id ::= DEVICE_ANYID.
device_id ::= .

device_attributes ::= device_attributes device_attribute.
device_attributes ::= .

device_attribute ::= KEYWORD_HASH string(S). {
  try {
    state->rule.setDeviceHash(*S);
    delete S;
  }
  catch(...) {
    state->error.setHint("Invalid device hash value: " + *S);
    delete S;
    throw state->error;
  }
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

usbiftype(T) ::= INTERFACE_TYPE_FULL(I). {
  StringVector tokens;
  tokenizeString(quex::unicode_to_char(I->get_text()), tokens, ":", /*trim_empty=*/false);

  T = new USBInterfaceType(stringToNumber<uint8_t>(tokens[0], 16),
                           stringToNumber<uint8_t>(tokens[1], 16),
                           stringToNumber<uint8_t>(tokens[2], 16));
  delete I;
}

usbiftype(T) ::= INTERFACE_TYPE_SUBCLASS(I). {
  StringVector tokens;
  tokenizeString(quex::unicode_to_char(I->get_text()), tokens, ":", /*trim_empty=*/false);
  T = new USBInterfaceType(stringToNumber<uint8_t>(tokens[0], 16),
                           stringToNumber<uint8_t>(tokens[1], 16),
                           0,
                           USBInterfaceType::MatchClass|USBInterfaceType::MatchSubClass),
  delete I;
}

usbiftype(T) ::= INTERFACE_TYPE_CLASS(I). {
  StringVector tokens;
  tokenizeString(quex::unicode_to_char(I->get_text()), tokens, ":", /*trim_empty=*/false);
  T = new USBInterfaceType(stringToNumber<uint8_t>(tokens[0], 16),
                           0, 0, USBInterfaceType::MatchClass);
  delete I;
}

usbiftypevec(D) ::= usbiftypevec(S) usbiftype(V). {
	D = S;
	D->push_back(*V);
	delete V;
}

usbiftypevec(V) ::= . {
	V = new std::vector<USBInterfaceType>();
}

condition_set_op(O) ::= SET_OPERATOR(V). {
  O = Rule::setOperatorFromString(quex::unicode_to_char(V->get_text()));
  delete V;
}

condition_set_op(O) ::= . {
  O = Rule::SetOperator::EqualsOrdered;
}

negation_op(N) ::= NEGATION. {
  N = true;
}

negation_op(N) ::= . {
  N = false;
}

condition(C) ::= negation_op(N) CONDITION_IDENTIFIER(I) PQ_STRING_BEGIN PQ_STRING(P) PQ_STRING_END. {
  const String identifier = quex::unicode_to_char(I->get_text());
  const String parameter = quex::unicode_to_char(P->get_text());
  const bool negated = N;
  C = RuleCondition::getImplementation(identifier, parameter, negated);
  delete I;
  delete P;
}

condition(C) ::= negation_op(N) CONDITION_IDENTIFIER(I). {
  const String identifier = quex::unicode_to_char(I->get_text());
  const String parameter;
  const bool negated = N;
  C = RuleCondition::getImplementation(identifier, parameter, negated);
  delete I;
}

conditionvec(D) ::= conditionvec(S) condition(V). {
  D = S;
  D->push_back(V->clone());
  delete V;
}

conditionvec(V) ::= . {
  V = new std::vector<RuleCondition*>();
}

conditions ::= conditions condition_spec.
conditions ::= .

condition_spec ::= KEYWORD_IF condition(C). {
  state->rule.internal()->refConditions().push_back(C);
  state->rule.setConditionSetOperator(Rule::SetOperator::EqualsOrdered);
}

condition_spec ::= KEYWORD_IF condition_set_op(O) CURLYBRACE_OPEN conditionvec(V) CURLYBRACE_CLOSE. {
  state->rule.internal()->refConditions().insert(state->rule.internal()->refConditions().end(),
                                                 V->begin(), V->end());
  state->rule.setConditionSetOperator(O);
}

string(S) ::= DQ_STRING_BEGIN DQ_STRING(V) DQ_STRING_END. {
	  const String from_unicode = quex::unicode_to_char(V->get_text());
	  const String unescaped = Rule::unescapeString(from_unicode);
	  S = new String(unescaped);
	  delete V;
}

