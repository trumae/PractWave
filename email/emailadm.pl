#!/usr/bin/perl

use Mail::POP3Client;
use IO::Socket::SSL;
use Email::Send;
use Email::Send::Gmail;
use Email::Simple::Creator;

# pop3 host
$pop_host = "pop.gmail.com";
# pop3 username (for Gmail, I didn't have to put @gmail.com at the end)
$pop_user = "sysadm\@practwave.com.br";
# pop3 password
$pop_pass = "PractWave1234";
# ssl port number (995 is what Gmail uses)
$ssl_port = "995";
# ssl protocol
$ssl_prot = "tcp";
# end of configuration

sub sendEmail {
    my ($to, $subject, $conteudo) = @_;

    my $email = Email::Simple->create(
	header => [
	    From    => 'sysadm@practwave.com.br',
	    To      => $to,
	    Subject => $subject,
	],
	body => $conteudo,
	);
    
    my $sender = Email::Send->new(
	{   mailer      => 'Gmail',
	    mailer_args => [
		username => $pop_user,
		password => $pop_pass,
		]
	}
	);
    eval { $sender->send($email) };
    die "Error sending email: $@" if $@;
}

sub processaHELP {
   my($to) = @_;
   my $help = <<EOF
Comandos aceitos pelo sysadm:
HELP - instrucoes de uso;
ACEITATODOSCONVITES - comando que aceita todos os convites de novos usuarios pendentes e retorna a lista de mail desses usuarios;
RELATORIOGERAL - informacoes basicas sobre uso do sistema
EOF
;
    sendEmail($to, "HELP", $help);
}

sub processaACEITATODOSCONVITES {
   my($to) = @_;
   my $conteudo = `./aceitaTodosConvitesPendentes.pl`;
   sendEmail($to, "Convites aceitos", $conteudo);
}

sub processaRELATORIOGERAL {
   my($to) = @_;
   my $conteudo = `./report.pl`;
   sendEmail($to, "Informacoes gerais", $conteudo);
}

while(1) {
    print "Processando\n";
    
    my $socket = IO::Socket::SSL->new( PeerAddr => $pop_host,
				       PeerPort => $ssl_port,
				       Proto    => $ssl_prot);
    my $pop = Mail::POP3Client->new();
    $pop->User($pop_user);
    $pop->Pass($pop_pass);
    $pop->Socket($socket);
    $pop->Connect();
    
    $msg_count = $pop->Count();
    print "$msg_count\n";
    
    
    for (my $i = 1; $i <= $msg_count; $i ++) {
	my $aprovado = 0;
	my $from;
	my $subject;
	foreach ( $pop->Head( $i ) ) {
	    #/^(From|Subject):\s+/i and print $_, "\n";
	    if ($_ =~ m/^From:/) {
		($from) = ($_ =~ m#^From: .*<(.*)>#);
		$from = substr($from, 0, 30);
		if($from eq "trumae\@gmail.com" ||
		   $from eq "willianfarago\@gmail.com") {
		   $aprovado = 1;
	        } 
	    }
	    if ($_ =~ m/^Subject:/) {
		($subject) = ($_ =~ m#^Subject: (.*)#);
	    }
	}

	if($aprovado) {
	    print "Aprovado eh respondendo para $from com subject = [$subject]\n";
	    if($subject =~ m/HELP/){
	        processaHELP($from);
	    }
            if($subject =~ m/ACEITATODOSCONVITES/){
	        processaACEITATODOSCONVITES($from);
	    }
            if($subject =~ m/RELATORIOGERAL/){
	        processaRELATORIOGERAL($from);
	    }
	}

	my $body =  $pop->Body($i);
	
	#deleta mensagem
	#$pop->Delete($i);
    }
    
    $pop->Close();
    sleep(5);
}



