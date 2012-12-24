#!/usr/bin/perl -W

use DBI;
use strict;

my @dbs = `ls ../*.db`;
my $numdbs = scalar(@dbs) - 2; #retira ads.db e user.db

sub cabecalho {
print <<EOF
=======================================
        Relatorios PractWave 
=======================================
EOF
;
}

sub rel1 {
   my $totalClientes = 0;
   my $maisClientes;
   my $cmaisClientes = 0;

   my $totalContasBancarias = 0;
   my $maisContasBancarias;
   my $cmaisContasBancarias = 0;

   my $totalLancamentos = 0;
   my $maisLancamentos;
   my $cmaisLancamentos = 0;

   foreach my $db (@dbs){
     chomp $db;
     if ( not ($db =~ /ads\.db$/ ||
	       $db =~ /email\.db$/ ||
	       $db =~ /user\.db$/)) {
        my $dbh = DBI->connect("dbi:SQLite:dbname=$db","","");

	# conta clientes
	my ($ccli) = $dbh->selectrow_array("
	    SELECT COUNT(*)
	    FROM cliente");
        $totalClientes += $ccli;
	if($ccli > $cmaisClientes){
	    $cmaisClientes = $ccli;
	    $maisClientes = $db;
	}

	# conta contas bancarias 
	my ($cbanco) = $dbh->selectrow_array("
	    SELECT COUNT(*)
	    FROM contabancaria");
        $totalContasBancarias += $cbanco;
	if($cbanco > $cmaisContasBancarias){
	    $cmaisContasBancarias = $cbanco;
	    $maisContasBancarias = $db;
	}

	# conta lancamentos 
	my ($clanc) = $dbh->selectrow_array("
	    SELECT COUNT(*)
	    FROM diario");
        $totalLancamentos += $clanc;
	if($clanc > $cmaisLancamentos){
	    $cmaisLancamentos = $clanc;
	    $maisLancamentos = $db;
	}

        $dbh->disconnect; 
     }
   }
   print "Total de clientes cadastrados: $totalClientes\n";
   print "\tMais clientes cadastrados ($cmaisClientes): $maisClientes\n";

   print "Total de contas bancarias cadastrados: $totalContasBancarias\n";
   print "\tMais contas bancarias cadastrados ($cmaisContasBancarias): $maisContasBancarias\n";

   print "Total de lancamentos realizados: $totalLancamentos\n";
   print "\tMais lancamentos realizados ($cmaisLancamentos): $maisLancamentos\n";
}

cabecalho();
print "numero de bancos de dados: $numdbs\n";
rel1();

