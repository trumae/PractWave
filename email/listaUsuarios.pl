#! /usr/bin/perl -W

use strict;
use DBI;

my @ls = `ls -la *.db`;

#retira user.db
my $index = 0;
$index++ until $ls[$index] =~ /user\.db/;
splice(@ls, $index, 1);


#retira user.db
$index = 0;
$index++ until $ls[$index] =~ /ads\.db/;
splice(@ls, $index, 1);

#retira user.db
$index = 0;
$index++ until $ls[$index] =~ /ads2\.db/;
splice(@ls, $index, 1);

my @lsExtendido;

sub getExtraInfo {
   my ($file) = @_;

   my $dbh = DBI->connect("dbi:SQLite:dbname=$file","","");
   my ($ctimeline) = $dbh->selectrow_array("SELECT COUNT(*) FROM timeline");
   my ($cmes) = $dbh->selectrow_array("select count(*) from timeline where timestamp > date('now', '-1 month')");
   my ($csemana) = $dbh->selectrow_array("select count(*) from timeline where timestamp > date('now', '-7 day')");
   $dbh->disconnect;

   return "$ctimeline $csemana $cmes ";
}

foreach my $linha (@ls) {
   my ($perm, $n, $usuario, $grupo, $tam, $mes, $dia, $hora, $email) = split(/\s+/, $linha); 
   my $num = getExtraInfo($email);
   push (@lsExtendido, "$num $linha"); 
}

sub maiorArquivo {
   my ($num ,$perm, $n, $usuario, $grupo, $tam, $mes, $dia, $hora, $email) = split(/\s+/, $a); 
   my ($num2 ,$perm2, $n2, $usuario2, $grupo2, $tam2, $mes2, $dia2, $hora2, $email2) = split(/\s+/, $b);
   return $num2 <=> $num; 
}

my @lsOrderTamArquivo = sort maiorArquivo @lsExtendido;

foreach my $linha (@lsOrderTamArquivo) {
   my ($num, $tweek, $tmonth, $perm, $n, $usuario, $grupo, $tam, $mes, $dia, $hora, $email) = split(/\s+/, $linha); 

format STDOUT_TOP =
                    Relatorio de  Uso
Banco                       bytes   TrasTotial  TransSema TransMes
---------------------------------------------------------------------
.

format STDOUT =
@<<<<<<<<<<<<<<<<<<<<<<<<< @<<<<<<  @<<<<<<<<   @<<<<<<<< @<<<<<<
$email,                    $tam,    $num,       $tweek,   $tmonth
.
   write();
}


