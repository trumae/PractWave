#!/usr/bin/perl -W

use strict;
use DBI;
use CabureEmail;

my $db = CabureEmail::initCabureEmail();
my $dbuser = DBI->connect("dbi:SQLite:../user.db","",""); 

my $stmt = $dbuser->prepare("select name, email from user where conviteaceito = 0");
$stmt->execute or die "Nao consigo executar comando sql. Erro: $DBI::errstr\n ";
my $row;
$db->{AutoCommit} = 0;
$db->{RaiseError} = 1;
$dbuser->{AutoCommit} = 0;
$dbuser->{RaiseError} = 1;

eval{
    while($row = $stmt->fetchrow_hashref()){
	    $db->do("insert into email (id, toemail, tipo, criadoem, enviado,script) values " . 
	    	    "(NULL, '$row->{email}', 'N', datetime('now'), 0, 'bemvindo.pl')");
	    $dbuser->do("update user set conviteaceito = 1 where email = '$row->{email}'");
	    print "Aceitando $row->{email}\n";
    }
    $db->commit();
    $dbuser->commit();
};

if ($@) {
    warn "Transaction aborted because $@";
    # now rollback to undo the incomplete changes
    # but do it in an eval{} as it may also fail
    eval { $db->rollback };
    eval { $dbuser->rollback };
    # add other application on-error-clean-up code here
}


