/**
 ******************************************************************************
 * @file     : lib_httpclient.cpp
 * @author   : robin
 * @version  : V1.0.0
 * @date     : 6-December-2014
 * @brief    :     
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */
#include "lib_httpclient.h"

/*********************************************************************************
  *Function          :    HttpClient::HttpClient()   
  *Description      :    constructor function
  *Input              :
  *Output            :         
  *Return            :        
  *author            :        
  *date               :           
  *Others            :     
**********************************************************************************/
HttpClient::HttpClient() :
    insecure(false) 
{
    // Empty
}

/*********************************************************************************
  *Function          :  unsigned int HttpClient::get(String &url) 
  *Description      :  A wrapper for the cURL command get.The method gets the specified URL from a server.
  *Input              :  url : a string containing the url to retrieve
  *Output            :  none
  *Return            :  the result
  *author            :  robot   
  *date               :  2015-02-01  
  *Others            :      
**********************************************************************************/
unsigned int HttpClient::get(String &url) 
{
    begin("curl");
    if (insecure) 
    {
        addParameter("-k");
    }
    addHeader();
    addParameter(url);
    return run();
}

/*********************************************************************************
  *Function          :   unsigned int HttpClient::get(const char *url) 
  *Description      :   A wrapper for the cURL command get.The method gets the specified URL from a server.
  *Input              :   url : a string containing the url to retrieve
  *Output            :   none
  *Return            :   the result
  *author            :   robot  
  *date               :   2015-02-01 
  *Others            :       
**********************************************************************************/
unsigned int HttpClient::get(const char *url) 
{
    begin("curl");
    if (insecure) 
    {
        addParameter("-k");
    }
    addHeader();
    addParameter(url);
    return run();
}

/*********************************************************************************
  *Function          :   void HttpClient::getAsynchronously(String &url) 
  *Description      :   A wrapper for the cURL command get. This command runs asynchronously and is non-blocking
                             The method gets the specified URL from a server.
  *Input              :   url : a string containing the url to retrieve
  *Output            :   none
  *Return            :   none
  *author            :   robot
  *date               :   2015-02-01 
  *Others            :            
**********************************************************************************/
void HttpClient::getAsynchronously(String &url) 
{
    begin("curl");
    if (insecure) 
    {
        addParameter("-k");
    }
    addHeader();
    addParameter(url);
    runAsynchronously();
}

/*********************************************************************************
  *Function          :   void HttpClient::getAsynchronously(String &url) 
  *Description      :   A wrapper for the cURL command get. This command runs asynchronously and is non-blocking
                             The method gets the specified URL from a server.
  *Input              :   url : a string containing the url to retrieve
  *Output            :   none
  *Return            :   none
  *author            :   robot
  *date               :   2015-02-01        
  *Others            :       
**********************************************************************************/
void HttpClient::getAsynchronously(const char *url) 
{
    begin("curl");
    if (insecure) 
    {
        addParameter("-k");
    }
    addHeader();
    addParameter(url);
    runAsynchronously();
}

/*********************************************************************************
  *Function          :   unsigned int HttpClient::post(String &url, String &data) 
  *Description      :   A wrapper for the cURL command post.
  *Input              :
  *Output            :     
  *Return            :        
  *author            :        
  *date               :           
  *Others            :      
**********************************************************************************/
unsigned int HttpClient::post(String &url, String &data) 
{
    return post(url.c_str(), data.c_str());
}

/*********************************************************************************
  *Function          :   unsigned int HttpClient::post(const char *url, const char *data) 
  *Description      :   A wrapper for the cURL command post.
  *Input              :
  *Output            :     
  *Return            :        
  *author            :        
  *date               :           
  *Others            :         
**********************************************************************************/
unsigned int HttpClient::post(const char *url, const char *data) 
{
    begin("curl");
    if (insecure) 
    {
        addParameter("-k");
    }
    addParameter("--request");
    addParameter("POST");
    addParameter("--data");
    addParameter(data);
    addHeader();
    addParameter(url);
    return run();
}

/*********************************************************************************
  *Function          :   void HttpClient::postAsynchronously(String &url, String &data) 
  *Description      :   A wrapper for the cURL command post. Asynchronously
  *Input              :
  *Output            :     
  *Return            :        
  *author            :        
  *date               :           
  *Others            :           
**********************************************************************************/
void HttpClient::postAsynchronously(String &url, String &data) 
{
    postAsynchronously(url.c_str(), data.c_str());
}

/*********************************************************************************
  *Function          :   void HttpClient::postAsynchronously(const char *url, const char *data) 
  *Description      :   A wrapper for the cURL command post. Asynchronously
  *Input              :
  *Output            :     
  *Return            :        
  *author            :        
  *date               :           
  *Others            :          
**********************************************************************************/
void HttpClient::postAsynchronously(const char *url, const char *data) 
{
    begin("curl");
    if (insecure) 
    {
        addParameter("-k");
    }
    addParameter("--request");
    addParameter("POST");
    addParameter("--data");
    addParameter(data);
    addHeader();
    addParameter(url);
    runAsynchronously();
}

/*********************************************************************************
  *Function          :  boolean HttpClient::ready(void)  
  *Description      :  Checks if an HTTP request initiated with getAsynchronously() is still in progess.
  *Input              :  none
  *Output            :  none
  *Return            :  boolean false if the request is still in progess, true if the request is completed     
  *author            :  robot
  *date               :  2015-02-01  
  *Others            :          
**********************************************************************************/
boolean HttpClient::ready(void) 
{
    return !running();
}

/*********************************************************************************
  *Function          :  unsigned int HttpClient::getResult(void) 
  *Description      :  Checks a process started by runAsynchronously() to see if it is still running.
  *Input              :   none
  *Output            :   none
  *Return            :   see also exitValue()   
  *author            :   robot     
  *date               :   2015-02-01 
  *Others            :          
**********************************************************************************/
unsigned int HttpClient::getResult(void) 
{
    return exitValue();
}


/*********************************************************************************
  *Function          :   void HttpClient::noCheckSSL(void) 
  *Description      :
  *Input              :
  *Output            :     
  *Return            :        
  *author            :        
  *date               :           
  *Others            :         
**********************************************************************************/
void HttpClient::noCheckSSL(void) 
{
    insecure = true;
}

/*********************************************************************************
  *Function          :   void HttpClient::checkSSL(void)  
  *Description      :
  *Input              :
  *Output            :     
  *Return            :        
  *author            :        
  *date               :           
  *Others            :      

**********************************************************************************/
void HttpClient::checkSSL(void) 
{
    insecure = false;
}

/*********************************************************************************
  *Function          :    void HttpClient::setHeader(String &header) 
  *Description      :
  *Input              :
  *Output            :     
  *Return            :        
  *author            :        
  *date               :           
  *Others            :         
**********************************************************************************/
void HttpClient::setHeader(String &header) 
{
    this->header = header;
}

/*********************************************************************************
  *Function          :   void HttpClient::setHeader(const char * header) 
  *Description      :
  *Input              :
  *Output            :     
  *Return            :        
  *author            :        
  *date               :           
  *Others            :          
**********************************************************************************/
void HttpClient::setHeader(const char * header) 
{
    this->header = String(header);
}

/*********************************************************************************
  *Function          :   void HttpClient::addHeader(void) 
  *Description      :
  *Input              :
  *Output            :     
  *Return            :        
  *author            :        
  *date               :           
  *Others            :        
**********************************************************************************/
void HttpClient::addHeader(void) 
{
    if (header.length() > 0) 
    {
        addParameter("--header");
        addParameter(header);
    }
}

