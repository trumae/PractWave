#!/usr/bin/perl -W

use strict;
use DBI;
use CabureEmail;

my $nargs = scalar(@ARGV);
if ($nargs < 2) {
    die "usage: criaNotificacao.pl <to> <script> <par1> ... <par10>\n";
}

my $to = $ARGV[0];
my $script = $ARGV[1];
my $db = CabureEmail::initCabureEmail();
$db->{AutoCommit} = 0;
$db->{RaiseError} = 1;
eval{
    $db->do("insert into email (id, toemail, tipo, criadoem, enviado,script) values " . 
	    "(NULL, '$to', 'N', datetime('now'), 0, '$script')");
    $db->commit();
};

if ($@) {
    warn "Transaction aborted because $@";
    eval { $db->rollback };
}


