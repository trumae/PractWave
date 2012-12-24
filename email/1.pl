#!/usr/bin/perl -W

use strict;
use CabureEmail;

my $toemail = $ARGV[0];

my $corpo="Teste de mail pelo script de notificacao";
CabureEmail::sendEmail($toemail,"teste", $corpo);
