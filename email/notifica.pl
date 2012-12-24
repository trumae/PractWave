#!/usr/bin/perl -W

use strict;
use DBI;
use CabureEmail;

my $db = CabureEmail::initCabureEmail();

while(1){
my $stmt = $db->prepare("select id, toemail, script from email where tipo='N' and enviado == 0");

$stmt->execute or die "Nao consigo executar comando sql. Erro: $DBI::errstr\n ";
my $row;
while($row = $stmt->fetchrow_hashref()){
    print "Enviando email $row->{id} para $row->{toemail} pelo script $row->{script}\n";
    `./$row->{script} $row->{toemail} `;
    $db->do("update email set enviado = 1, enviadoem = datetime('now') where id = $row->{id} ");
}
warn "Retorno dos dados do banco terminado por erro: $DBI::errstr\n" 
   if $DBI::err;
   sleep(60);
}
