#!/usr/bin/perl -W

use strict;
use CabureEmail;

my $toemail = $ARGV[0];

my $corpo = <<END
From: practwave\@practwave.com.br 
To: $toemail
Subject: Subject Teste
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
    <meta property="og:title" content="Buzzero.com | E-mail" />
    <title>PractWave.com.br | E-mail</title>

	  <style type="text/css">
	  /* Client-specific Styles */
	      #outlook a{padding:0;} /* Force Outlook to provide a "view in browser" button. */
	      body{width:100% !important;} .ReadMsgBody{width:100%;} .ExternalClass{width:100%;} /* Force Hotmail to display emails at full width */
	      body{-webkit-text-size-adjust:none;} /* Prevent Webkit platforms from changing default text sizes. */
	      /* Reset Styles */
	      body{margin:0;padding:0;}
	      img{border:0; height:auto; line-height:100%; outline:none; text-decoration:none;}
	      table td{border-collapse:collapse;}
	      /* Template Styles */
	      body{color:#333;font-family:Arial;font-size: 14px;line-height: 150%;}
	  </style>
	  </head>
	  <body leftmargin="0" marginwidth="0" topmargin="0" marginheight="0" offset="0" style="color:#333;font-family:Arial;font-size:14px;line-height:150%;">
	     <center>
	       <table border="0" cellpadding="0" cellspacing="0" bgcolor="#ffffff" style="background-color: #ffffff; width: 600px;">
	          <tbody>


		  <tr>
		    <td>Conteudo Principal</td>
		  </tr>
                  </tbody>
               </table>  
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
		       <a href='http://blog.practwave.com.br'>Clique aqui</a> para ler instruções de como não receber mais os e-mails dessa lista.
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

