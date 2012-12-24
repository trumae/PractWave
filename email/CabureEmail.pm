package CabureEmail;

use DBI;

my $db;

sub initCabureEmail {
    $db = DBI->connect("dbi:SQLite:email.db","","");	
    
    $db->do("create table if not exists email (" .
	    " id integer primary key,  " .
	    " toemail text not null, " .
	    " tipo char(1)," .
	    " criadoem date not null default(datetime('now')), " .
	    " enviado integer default 0, " .
	    " enviadoem date, " .
	    " script text not null, " .
	    " par1 text, " .
	    " par2 text, " .
	    " par3 text, " .
	    " par4 text, " .
	    " par5 text, " .
	    " par6 text, " .
	    " par7 text, " .
	    " par8 text, " .
	    " par9 text, " .
	    " par10 text " .
	    ")" );
    return $db;
}

sub sendEmail {
    my ($to, $subject, $body) = @_;

    open(SEND, "| ./ses/ses-send-email.pl -k ./ses/aws-credentials " . 
       "-f practwave\@practwave.com.br " .
       "$to " .
       "-s '$subject' " 
    ) or
       die("Nao consigo enviar email!");
   print SEND $body;
   close(SEND);    
}

sub sendEmailRaw {
   my ($body) = @_;

   open(SEND, "| ./ses/ses-send-email.pl -k ./ses/aws-credentials -r ") or
       die("Nao consigo enviar email!");
   print SEND $body;
   close(SEND);    
}

1;

