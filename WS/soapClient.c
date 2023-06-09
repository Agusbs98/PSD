/* soapClient.c
   Generated by gSOAP 2.8.75 for conecta3x3.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"
#ifdef __cplusplus
extern "C" {
#endif

SOAP_SOURCE_STAMP("@(#) soapClient.c ver 2.8.75 2022-11-10 14:19:02 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_call_conecta3x3ns__register(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tMessage playerName, int *code)
{	if (soap_send_conecta3x3ns__register(soap, soap_endpoint, soap_action, playerName) || soap_recv_conecta3x3ns__register(soap, code))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_send_conecta3x3ns__register(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tMessage playerName)
{	struct conecta3x3ns__register soap_tmp_conecta3x3ns__register;
	if (soap_endpoint == NULL)
		soap_endpoint = "http//localhost:10000";
	soap_tmp_conecta3x3ns__register.playerName = playerName;
	soap_begin(soap);
	soap->encodingStyle = ""; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_conecta3x3ns__register(soap, &soap_tmp_conecta3x3ns__register);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_conecta3x3ns__register(soap, &soap_tmp_conecta3x3ns__register, "conecta3x3ns:register", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_conecta3x3ns__register(soap, &soap_tmp_conecta3x3ns__register, "conecta3x3ns:register", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_recv_conecta3x3ns__register(struct soap *soap, int *code)
{
	struct conecta3x3ns__registerResponse *soap_tmp_conecta3x3ns__registerResponse;
	if (!code)
		return soap_closesock(soap);
	soap_default_int(soap, code);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_conecta3x3ns__registerResponse = soap_get_conecta3x3ns__registerResponse(soap, NULL, "", NULL);
	if (!soap_tmp_conecta3x3ns__registerResponse || soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	if (code && soap_tmp_conecta3x3ns__registerResponse->code)
		*code = *soap_tmp_conecta3x3ns__registerResponse->code;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_conecta3x3ns__getStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tMessage playerName, struct tBlock *status)
{	if (soap_send_conecta3x3ns__getStatus(soap, soap_endpoint, soap_action, playerName) || soap_recv_conecta3x3ns__getStatus(soap, status))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_send_conecta3x3ns__getStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tMessage playerName)
{	struct conecta3x3ns__getStatus soap_tmp_conecta3x3ns__getStatus;
	if (soap_endpoint == NULL)
		soap_endpoint = "http//localhost:10000";
	soap_tmp_conecta3x3ns__getStatus.playerName = playerName;
	soap_begin(soap);
	soap->encodingStyle = ""; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_conecta3x3ns__getStatus(soap, &soap_tmp_conecta3x3ns__getStatus);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_conecta3x3ns__getStatus(soap, &soap_tmp_conecta3x3ns__getStatus, "conecta3x3ns:getStatus", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_conecta3x3ns__getStatus(soap, &soap_tmp_conecta3x3ns__getStatus, "conecta3x3ns:getStatus", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_recv_conecta3x3ns__getStatus(struct soap *soap, struct tBlock *status)
{
	struct conecta3x3ns__getStatusResponse *soap_tmp_conecta3x3ns__getStatusResponse;
	if (!status)
		return soap_closesock(soap);
	soap_default_conecta3x3ns__tBlock(soap, status);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_conecta3x3ns__getStatusResponse = soap_get_conecta3x3ns__getStatusResponse(soap, NULL, "", NULL);
	if (!soap_tmp_conecta3x3ns__getStatusResponse || soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	if (status && soap_tmp_conecta3x3ns__getStatusResponse->status)
		*status = *soap_tmp_conecta3x3ns__getStatusResponse->status;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_conecta3x3ns__insertChip(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tMessage playerName, int pos, struct tBlock *status)
{	if (soap_send_conecta3x3ns__insertChip(soap, soap_endpoint, soap_action, playerName, pos) || soap_recv_conecta3x3ns__insertChip(soap, status))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_send_conecta3x3ns__insertChip(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tMessage playerName, int pos)
{	struct conecta3x3ns__insertChip soap_tmp_conecta3x3ns__insertChip;
	if (soap_endpoint == NULL)
		soap_endpoint = "http//localhost:10000";
	soap_tmp_conecta3x3ns__insertChip.playerName = playerName;
	soap_tmp_conecta3x3ns__insertChip.pos = pos;
	soap_begin(soap);
	soap->encodingStyle = ""; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_conecta3x3ns__insertChip(soap, &soap_tmp_conecta3x3ns__insertChip);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_conecta3x3ns__insertChip(soap, &soap_tmp_conecta3x3ns__insertChip, "conecta3x3ns:insertChip", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_conecta3x3ns__insertChip(soap, &soap_tmp_conecta3x3ns__insertChip, "conecta3x3ns:insertChip", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_recv_conecta3x3ns__insertChip(struct soap *soap, struct tBlock *status)
{
	struct conecta3x3ns__insertChipResponse *soap_tmp_conecta3x3ns__insertChipResponse;
	if (!status)
		return soap_closesock(soap);
	soap_default_conecta3x3ns__tBlock(soap, status);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_conecta3x3ns__insertChipResponse = soap_get_conecta3x3ns__insertChipResponse(soap, NULL, "", NULL);
	if (!soap_tmp_conecta3x3ns__insertChipResponse || soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	if (status && soap_tmp_conecta3x3ns__insertChipResponse->status)
		*status = *soap_tmp_conecta3x3ns__insertChipResponse->status;
	return soap_closesock(soap);
}

#ifdef __cplusplus
}
#endif

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapClient.c */
