#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <curl/curl.h>
#include "libxml2/libxml/HTMLparser.h"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

//
//  Case-insensitive string comparison
//

#ifdef _MSC_VER
#define COMPARE(a, b) (!_stricmp((a), (b)))
#else
#define COMPARE(a, b) (!strcasecmp((a), (b)))
#endif

//
//  libxml callback context structure
//

struct Context
{
  Context(): addTitle(false) { }

  bool addTitle;
  std::string title;
};

//
//  libcurl variables for error strings and returned data

static char errorBuffer[CURL_ERROR_SIZE];
static std::string buffer;

//
//  libcurl write callback function
//

static int writer(char *data, size_t size, size_t nmemb,
                  std::string *writerData)
{
  if(writerData == NULL)
    return 0;

  writerData->append(data, size*nmemb);

  return size * nmemb;
}

//
//  libcurl connection initialization
//

static bool init(CURL *&conn, char *url)
{
  CURLcode code;

  conn = curl_easy_init();

  if(conn == NULL) {
    fprintf(stderr, "Failed to create CURL connection\n");
    exit(EXIT_FAILURE);
  }

  code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
  if(code != CURLE_OK) {
    fprintf(stderr, "Failed to set error buffer [%d]\n", code);
    return false;
  }

  code = curl_easy_setopt(conn, CURLOPT_URL, url);
  if(code != CURLE_OK) {
    fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
    return false;
  }

  code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1L);
  if(code != CURLE_OK) {
    fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);
    return false;
  }

  code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
  if(code != CURLE_OK) {
    fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
    return false;
  }

  code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &buffer);
  if(code != CURLE_OK) {
    fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
    return false;
  }

  return true;
}

//
//  libxml start element callback function
//

static void StartElement(void *voidContext,
                         const xmlChar *name,
                         const xmlChar **attributes)
{
  Context *context = static_cast<Context *>(voidContext);

//  std::printf("<%s>", name);
  if(COMPARE(reinterpret_cast<const char *>(name), "TITLE")) {
    context->title = "";
    context->addTitle = true;
  }
  (void) attributes;
}

//
//  libxml end element callback function
//

static void EndElement(void *voidContext,
                       const xmlChar *name)
{
  Context *context = static_cast<Context *>(voidContext);

//  std::printf("</%s>\n", name);
  if(COMPARE(reinterpret_cast<const char *>(name), "TITLE"))
    context->addTitle = false;
}

//
//  Text handling helper function
//

static void handleCharacters(Context *context,
                             const xmlChar *chars,
                             int length)
{
//  std::printf("%s", chars);
  if(context->addTitle)
    context->title.append(reinterpret_cast<const char *>(chars), length);
}

//
//  libxml PCDATA callback function
//

static void Characters(void *voidContext,
                       const xmlChar *chars,
                       int length)
{
  Context *context = static_cast<Context *>(voidContext);

  handleCharacters(context, chars, length);
}

//
//  libxml CDATA callback function
//

static void cdata(void *voidContext,
                  const xmlChar *chars,
                  int length)
{
  Context *context = static_cast<Context *>(voidContext);

  handleCharacters(context, chars, length);
}

//
//  libxml SAX callback structure
//

static htmlSAXHandler saxHandler =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        StartElement,
        EndElement,
        NULL,
        Characters,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        cdata,
        NULL
    };

//
//  Parse given (assumed to be) HTML text and return the title
//

static void parseHtml(const std::string &html,
                      std::string &title)
{
  htmlParserCtxtPtr ctxt;
  Context context;

  ctxt = htmlCreatePushParserCtxt(&saxHandler, &context, "", 0, "",
                                  XML_CHAR_ENCODING_NONE);

  htmlParseChunk(ctxt, html.c_str(), html.size(), 0);
  htmlParseChunk(ctxt, "", 0, 1);

  htmlFreeParserCtxt(ctxt);



  title = context.title;
}

static void
print_element_names(xmlNode * a_node, int indent = 0)
{
  xmlNode *cur_node = NULL;
  std::string ind(indent, ' ');

  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      printf("\n%s", ind.c_str());
      printf("<%s", cur_node->name);

      for(auto attr = cur_node->properties; attr; attr = attr->next) {
        printf(" %s=", attr->name);
        if(attr->children)
          printf("\'%s\'", attr->children->content);
      }
      printf(">");
//      printf("node type: Element, name: %s, content:%s\n", cur_node->name, cur_node->content);
    }
    if(cur_node->content) {
      if (cur_node->content[0] != ' ' && strnlen(reinterpret_cast<const char *>(cur_node->content), 2) > 1){
        printf("\n%s", ind.c_str());
        printf("%s", cur_node->content);
        if (strncmp(reinterpret_cast<const char *>(cur_node->content), "관심주제 설정", 10) == 0) {
          int debug = 0;
        }
      }
    }
    print_element_names(cur_node->children, indent + 2);
    if(cur_node->type == XML_ELEMENT_NODE){
      if(cur_node->children)
        printf("\n%s", ind.c_str());
      printf("</%s>", cur_node->name);
    }
  }
}

int main(int argc, char *argv[])
{
  CURL *conn = NULL;
  CURLcode code;
  std::string title;

  // Ensure one argument is given

  char* url = "https://naver.com";

  curl_global_init(CURL_GLOBAL_DEFAULT);

  // Initialize CURL connection

  if(!init(conn, url)) {
    fprintf(stderr, "Connection initializion failed\n");
    exit(EXIT_FAILURE);
  }

  // Retrieve content for the URL

  code = curl_easy_perform(conn);
  curl_easy_cleanup(conn);

  if(code != CURLE_OK) {
    fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
    exit(EXIT_FAILURE);
  }

  // Parse the (assumed) HTML code
  parseHtml(buffer, title);
  htmlDocPtr doc = htmlParseDoc(reinterpret_cast<const xmlChar *>(buffer.c_str()), 0);
  if (doc == NULL) {
    std::cerr << "ERR\n";
    return -1;
  }

  auto root_elem = xmlDocGetRootElement(doc);
  print_element_names(root_elem);

//  std::cout << buffer << std::endl;

  // Display the extracted title
//  printf("Title: %s\n", title.c_str());

  return EXIT_SUCCESS;
}