#!/usr/bin/perl -W

use strict;
use CabureEmail;

my $toemail = $ARGV[0];

my $corpo = <<END
From: practwave\@practwave.com.br 
To: $toemail
Subject: Use o PractWave para gerenciar clientes  
MIME-Version: 1.0
Content-Type: multipart/mixed; boundary="==boundary=="

This is a message with multiple parts in MIME format.
--==boundary==
Content-Type: text/html

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <!-- Facebook sharing information tags -->
    <meta property="og:title" content="practwave.com.br | E-mail" />
    <meta name="viewport" property="width=device-width, initial-scale=1.0" />
    <title>PractWave.com.br | E-mail</title>
	  <style type="text/css">
	     \@media screen and (max-width: 480px) {
		     table[class=emailwrapto100pc],
		     img[class=emailwrapto100pc] {
			     width:100% !important; height:auto !important;
		     }
             }
	  </style>
	  </head>
	  <body style="padding:0;margin:0;-webkit-text-size-adjust:none;-ms-text-size-adjust:100%;background-color:#ffffff;" bgcolor="#ffffff">
	     <table width="640" border="0" cellspacing="0" cellpadding="0" class="emailwrapto100pc">
               <tr>
	         <td>	       
Hoje em dia, a simples oferta de um produto ou serviço de qualidade não é o suficiente para o sucesso de um empreendimento. Você sabe que é fundamental um controle prático e organizado na hora de gerenciar clientes, para aperfeiçoar suas vendas e progredir com seu negócio. É aí que entra o Practwave!
<br><br>
No Practwave, gerenciar informações de clientes é muito fácil e, para provar isso, fizemos um simples passo a passo que você poderá seguir. Acesse 
<a href="http://blog.practwave.com.br/2012/11/use-o-practwave-para-gerenciar-clientes.html">aqui</a>! 
               <br><br>
	       Fique ligado no <a href='http://blog.practwave.com.br'>blog do Practwave</a> para mais novidades e informações!
               <br>
	       <br>
               Atenciosamente!
	       <br><br>
	       O time do Practwave

	       </td>
	     </tr>
	     <center>
               <table border="0" cellpadding="0" cellspacing="0" bgcolor="#ffffff" style="background-color: #ffffff; width: 600px;">
                  <tbody>
                    <tr>
		      <td align="center" valign="middle" style="padding: 10px 0; font-size: 11px; color: #707070;">
		  ATENÇÃO: Não responda a essa mensagem. Mensagens enviadas a esse endereço não são lidas por nossa equipe de atendimento. Caso deseje entrar em contato conosco, por favor, utiliza o email 
                  <a href='mailto:contato\@practwave.com.br'>contato\@practwave.com.br</a>.
		      </td>
		    </tr>
                    <tr>
                      <td align="right" valign="middle" style="padding: 10px 0px; font-size: 11px; color: #707070; text-align: center;"><a href="http://emailmarketer.buzzero.com/unsubscribe.php?M=1478938&N=730&L=4&C=54c969a16c4b7be11174f7944f108c19" target="_blank" style="color: #d55200;">		    
		       <a href='http://blog.practwave.com.br/2012/11/como-preencher-ou-modificar-seus-dados.html'>Clique aqui</a> para ler instruções de como não receber mais os e-mails dessa lista.
		      </td>
                    </tr> 
                  </tbody> 
               </table>
	     </center>


	  </body>
</html>
--==boundary==--

END
;

CabureEmail::sendEmailRaw($corpo);

