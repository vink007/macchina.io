//
// JSServletFilter.cpp
//
// $Id: //poco/1.4/OSP/JS/src/JSServletFilter.cpp#1 $
//
// Copyright (c) 2013-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier: Apache-2.0
//


#include "JSServletFilter.h"
#include "JSServletExecutor.h"
#include "Poco/StreamCopier.h"


namespace Poco {
namespace OSP {
namespace JS {


namespace
{
	std::string htmlize(const std::string& str)
	{
		std::string::const_iterator it(str.begin());
		std::string::const_iterator end(str.end());
		std::string html;
		for (; it != end; ++it)
		{
			switch (*it)
			{
			case '<': html += "&lt;"; break;
			case '>': html += "&gt;"; break;
			case '"': html += "&quot;"; break;
			case '&': html += "&amp;"; break;
			default:  html += *it; break;
			}
		}
		return html;
	}
}


JSServletFilter::JSServletFilter(Poco::OSP::BundleContext::Ptr pContext):
	_pContext(pContext)
{
}


void JSServletFilter::process(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const std::string& path, std::istream& resourceStream, Poco::OSP::Bundle::ConstPtr pBundle)
{
	try
	{
		response.setContentType("text/html");
		std::string servlet;
		preprocess(request, response, path, resourceStream, servlet);
		std::string scriptURI("bndl://");
		scriptURI += pBundle->symbolicName();
		if (path.empty() || path[0] != '/') scriptURI += "/";
		scriptURI += path;
		JSServletExecutor::Ptr pServletExecutor = new JSServletExecutor(_pContext, pBundle, servlet, Poco::URI(scriptURI), request, response);
		pServletExecutor->run();
		if (!response.sent())
		{
			sendErrorResponse(response, "Script execution failed. See server log for details.");
		}
	}
	catch (Poco::Exception& exc)
	{
		_pContext->logger().log(exc);
		if (!response.sent())
		{
			sendErrorResponse(response, exc.displayText());
		}
	}
}


void JSServletFilter::preprocess(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const std::string& path, std::istream& resourceStream, std::string& servlet)
{
	Poco::StreamCopier::copyToString(resourceStream, servlet);
}


void JSServletFilter::sendErrorResponse(Poco::Net::HTTPServerResponse& response, const std::string& message)
{
	if (!response.sent())
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
		response.setChunkedTransferEncoding(true);
		response.setContentType("text/html");
		response.send()
			<< "<html>"
			<< "<head><title>" << response.getStatus() << ": " << response.getReason() << "</title></head>"
			<< "<body>"
			<< "<h1>" << response.getStatus() << ": " << response.getReason() << "</h1>"
			<< "<p>" << htmlize(message) << "</p>"
			<< "</body>"
			<< "</html>";
	}
}


} } } // namespace Poco::OSP::JS
