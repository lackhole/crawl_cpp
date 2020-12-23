#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "libxml2/libxml/HTMLparser.h"
#include <iostream>
#include <fstream>

#include "Curlpp.hpp"

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

// string that only contains control character and blanks return false
bool check_str(const char* cstr, std::size_t n = -1){
  if (cstr == NULL) return false;
  if (cstr[0] == ' ' && strnlen(cstr, 2) == 1) return false;

  auto len = strnlen(cstr, n);
  for (int i=0; i<len; ++i) {
    switch(cstr[i]){
      case '\t':
      case '\n':
      case ' ':
        break;
      default:
        return true;
    }
  }

  return false;
}

static void
print_element_names(xmlNode * a_node, int indent = 0)
{
  xmlNode *cur_node = NULL;
  std::string ind(indent, ' ');

  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      printf("\n%s", ind.c_str());
      printf("<");
//      printf("%s:", cur_node->ns);
      printf("%s", cur_node->name);

      for(auto attr = cur_node->properties; attr; attr = attr->next) {
        printf(" %s=", attr->name);
        if(attr->children)
          printf("\'%s\'", attr->children->content);
      }
      printf(">");
//      printf("node type: Element, name: %s, content:%s\n", cur_node->name, cur_node->content);
    }

    // it cannot detect ::before, ::after
    if(cur_node->content) {
      if(check_str(reinterpret_cast<const char *>(cur_node->content))) {
        printf("\n%s", ind.c_str());
        printf("%s", cur_node->content);
      }
      if (strncmp(reinterpret_cast<const char *>(cur_node->content), " window.nmain.gv", 10) == 0) {
        bool debug = true;
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

  const char* url = "https://foobar123123123afefae.com";
  std::string response;

  Curlpp curlpp;
  if(!curlpp.init() || !curlpp.request(url)){
    return 0;
  }

  response.resize(curlpp.bufferSize() + 1);
  curlpp.get(const_cast<char *>(response.data()), response.size());

  cout << response << endl;


  // Parse the (assumed) HTML code
//  parseHtml(buffer, title);
  htmlDocPtr doc = htmlParseDoc(reinterpret_cast<const xmlChar *>(response.c_str()), NULL);
  if (doc == NULL) {
    std::cerr << "ERR\n";
    return -1;
  }

  auto root_elem = xmlDocGetRootElement(doc);
  print_element_names(root_elem);

  // Display the extracted title
//  printf("Title: %s\n", title.c_str());

  return EXIT_SUCCESS;
}