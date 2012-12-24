#!/usr/bin/perl -W
use strict;

my $nargs = scalar(@ARGV);
if ($nargs != 4) {
    die "usage: criaPacotes.pl <id do promotor inicio> <id do promotor final> <numero de cupons por pacote> <validade>\n";
}

my $inicio = $ARGV[0];
my $fim = $ARGV[1];
my $numCupons = $ARGV[2];
my $validade = $ARGV[3];

for(my $i = $inicio; $i <= $fim; $i ++){
	print "Gerando pacote $i\n";
	`./criaCupons.pl $i $numCupons $validade`;
	`mv material.pdf material$i.pdf`;
}

