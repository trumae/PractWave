#!/usr/bin/perl -W

use strict;
use Digest::MD5;

my $corpoCartao = <<END
\\documentclass[11pt,a4paper]{memoir}

\\setstocksize{55mm}{85mm} % UK Stock size
\\setpagecc{55mm}{85mm}{*}
\\settypeblocksize{45mm}{75mm}{*}
\\setulmargins{5mm}{*}{*}
\\setlrmargins{5mm}{*}{*}


\\setheadfoot{0.1pt}{0.1pt}
\\setheaderspaces{1pt}{*}{*}

\\checkandfixthelayout[fixed]

\\pagestyle{empty}

\\usepackage{pstricks}
\\usepackage{graphicx}

\\begin{document}
    %\\pagecolor[cmyk]{.22,.36,.51,.08}%
    \\begin{Spacing}{0.50}%
    \\noindent
    \\textbf{Promo\\c{c}\\~{a}o PractWave} \\\\
    \\rule{74mm}{1mm}\\\\
    \\begin{minipage}[t]{30mm}
        \\vspace{-1mm}%
        \\begin{pspicture}(30mm,30mm)
		\\includegraphics[scale=0.4]{../imagens/icone256x256.png}
        \\end{pspicture}
    \\end{minipage}
    \\hspace{1mm}
    \\begin{minipage}[t]{42mm}
        \\vspace{-1mm}%
        \\begin{flushleft}
        {\\footnotesize
            \\begin{Spacing}{1}%
            \\vspace{1mm}
	    \\textbf{C\\'{o}digo promocional}\\\\
            CODIGOXXX\\vspace{1mm}\\
            \\end{Spacing}
        }
	{\\scriptsize
		Gerenciamento de neg\\'{o}cio para empreendedores individuais.
	    Gratuito!\\vspace{4mm}\\
	}
        {\\tiny

            http://www.practwave.com.br/\\
            \\textbf{validade:} VALIDADEXXX\\
            practwave\@practwave.com.br\\
            \\vspace*{2mm}
        }
        \\end{flushleft}
    \\end{minipage}
    \\rule{74mm}{1mm}
    \\end{Spacing}
\\end{document}
END
;

my $corpoCupons = <<END
\\documentclass{article}
\\textwidth 22.00cm % 21.59cm
\\textheight 27.94cm
\\topmargin -1.75in % was -1.0

\\pagestyle{empty}

\\headheight 0in
\\headsep 0in
\\oddsidemargin -0.5in % ok fits A4 paper

\\usepackage{pstricks} % allows using PSTricks!!  Remove if you don't have it.
\\usepackage{pst-node} % nodes in pst

\\usepackage{graphicx}
\\usepackage[pdftex]{hyperref}
\\usepackage{float}
\\usepackage{caption}
\\usepackage{subcaption}

\\pagestyle{empty} % removes page numbers
\\begin{document}
\\noindent
\\setlength{\\unitlength}{1in}

\\vspace{15mm}

\\centering
TEXTO
\\end{document}

END
;

sub getCodigo {
   my ($promotor, $contador, $validade) = @_;

   my $ret = "";
   $ret .= $promotor . chr(int(rand(23)) + 97);
   $ret .= $contador . chr(int(rand(23)) + 97);
   $ret .= $validade;
   my $hash = Digest::MD5::md5_hex($ret);
   $ret .= substr($hash,0, 2);
   return $ret;
}

sub criaUmCupom {
   my ($codigo, $validade) = @_;
 
   my $corpo = $corpoCartao;
   $corpo =~ s/CODIGOXXX/$codigo/;
   $corpo =~ s/VALIDADEXXX/$validade/;
   open(OUT, ">$codigo.tex") or die "Nao consigo criar arquivo $codigo.tex\n";
   print OUT $corpo;
   close OUT;
   `pdflatex $codigo.tex`;
   `rm -f *.log *.tex *.aux *.out`;
}


my $nargs = scalar(@ARGV);
if ($nargs != 3) {
    die "usage: criaCupom.pl <id do promotor> <numero de cupons> <validade>\n";
}

sub criaCupons {
   my ($id, $num, $val) = @_;
   my $corpo = $corpoCupons;
   my $texto = ""; 

   my @arqs = ();

   for(my $i = 1; $i < $num; $i++) {
      my $codigo = getCodigo($id, $i, $val);
      criaUmCupom("c" . $codigo, $val);
      
      $texto .= "\\begin{minipage}[h]{0.48\\textwidth}\n";
      $texto .= "   \\includegraphics{c$codigo.pdf}\n";
      $texto .= "\\end{minipage}\n";

      $texto .= "\\vspace{-5mm}\n" if ($i % 2 == 0);
      
      if($i % 10 == 0 && $i > 0) {
         $corpo =~ s/TEXTO/$texto/;
         open (OUT, ">material$i.tex") or die "Nao consigo criar arquivo material$i.tex\n";
         print OUT $corpo;
         close OUT;

         `pdflatex material$i.tex`;
	 push @arqs, "material$i.pdf"; 
	 $corpo = $corpoCupons;
	 $texto = "";
      }
   }

   `gs -q -sPAPERSIZE=a4paper -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -sOutputFile=material.pdf @arqs`;
   `rm -f @arqs`;
}

my $idpromotor = $ARGV[0];
my $numCupons = $ARGV[1];
my $validade = $ARGV[2];

criaCupons($idpromotor, $numCupons, $validade);

`rm -f c*.pdf *.log *.tex *.aux *.out`;

