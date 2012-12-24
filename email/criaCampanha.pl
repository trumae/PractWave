#!/usr/bin/perl -W

use strict;
use DBI;
use CabureEmail;

my $nargs = scalar(@ARGV);
if ($nargs < 1) {
   die "usage: criaCampanha.pl <script> <par1> ... <par10>\n";
}

my $script = $ARGV[0];
my $db = CabureEmail::initCabureEmail();

my $dbuser = DBI->connect("dbi:SQLite:../user.db","",""); 

my $stmt = $dbuser->prepare("select id, name, email from user");
$stmt->execute or die "Nao consigo executar comando sql. Erro: $DBI::errstr\n ";
my $row;
$db->{AutoCommit} = 0;
$db->{RaiseError} = 1;

eval{
    while($row = $stmt->fetchrow_hashref()){
	my $stmt2 = $dbuser->prepare("select autorizacao_email from dados_user where id_user = $row->{id}");
        $stmt2->execute or die "Nao consigo executar comando sql2. Erro: $DBI::errstr\n ";
        my $row2;
	if($row2 = $stmt2->fetchrow_hashref()){
	      if($row2->{autorizacao_email}){
	           $db->do("insert into email (id, toemail, tipo, criadoem, enviado,script) values " . 
	   	           "(NULL, '$row->{email}', 'C', datetime('now'), 0, '$script')");
	      }
        } else {
	           $db->do("insert into email (id, toemail, tipo, criadoem, enviado,script) values " . 
	   	           "(NULL, '$row->{email}', 'C', datetime('now'), 0, '$script')");
	}
    }
    $db->commit();
};

if ($@) {
    warn "Transaction aborted because $@";
    # now rollback to undo the incomplete changes
    # but do it in an eval{} as it may also fail
    eval { $db->rollback };
    # add other application on-error-clean-up code here
}


