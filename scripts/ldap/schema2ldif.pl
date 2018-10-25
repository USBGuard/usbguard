#!/usr/bin/env perl
# Converts OpenLDAP schema from traditional slapd.conf format to LDIF format
# usable for importing into cn=config.
#
# Copyright (c) 2012-2016 Mantas Mikulėnas <grawity@gmail.com>
# Released under the MIT license <https://spdx.org/licenses/MIT>

use warnings;
use strict;

my $name = shift(@ARGV) // "UNNAMED-SCHEMA";
my $unwrap = 0;

print "dn: cn=$name,cn=schema,cn=config\n";
print "objectClass: olcSchemaConfig\n";

my $key;
my $value;

while (<STDIN>) {
	if (/^(attributeType(?:s)?|objectClass(?:es)?) (.+)$/i) {
		if ($key && $value) {
			print "$key: $value\n";
		}
		($key, $value) = ($1, $2);
		if ($key =~ /^attributeType(s)?$/i) {
			$key = "olcAttributeTypes";
		} elsif ($key =~ /^objectClass(es)?$/i) {
			$key = "olcObjectClasses";
		} else {
			$key = "olc$key";
		}
	}
	elsif (/^\s+(.+)$/) {
		if ($unwrap) {
			$value .= " $1";
		} else {
			$value .= "\n $&";
		}
	}
	elsif (/^#.*/) {
		print "$&\n";
	}
	elsif (/.+/) {
		warn "$.:unrecognized input line: $&\n";
	}
}
if ($key && $value) {
	print "$key: $value\n";
}
