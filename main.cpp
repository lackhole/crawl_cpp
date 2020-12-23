#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "libxml2/libxml/HTMLparser.h"
#include "libxml2/libxml/xpath.h"
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "curlpp.hpp"

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

std::unordered_map<xmlAttrPtr, xmlNodePtr>
getKeyPair(xmlAttrPtr attr){
  std::unordered_map<xmlAttrPtr, xmlNodePtr> m;
  std::unordered_map<int*, int*> m2;

  for(; attr; attr = attr->next){
    m.emplace(attr, attr->children?attr->children:nullptr);
  }

  return m;
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

void printNode(xmlNode* node, int indent = 0) {
//  std::string indent_str(indent, ' ');

  if (node->type == XML_ELEMENT_NODE) {
//    if (indent) printf("\n%s", indent_str.c_str());

    printf("<");
    printf("%s", node->name);

    // print properties
    if(node->properties) {
      for (auto attr = node->properties; attr; attr = attr->next) {
        printf(" %s=", attr->name);
        if (attr->children)
          printf("\'%s\'", attr->children->content);
      }
    }

    printf(">");
  }

  if (node->content && check_str(reinterpret_cast<const char *>(node->content))) {
//    printf("\n%s", indent_str.c_str());
    printf("%s", node->content);
  }
  else if (node->children && node->children->type == XML_TEXT_NODE) {
    printf("%s", node->children->content);
  }

  if(node->type == XML_ELEMENT_NODE){
//    if(node->children)
//      printf("\n%s", indent_str.c_str());
    printf("</%s>\n", node->name);
  }
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




std::unique_ptr<xmlXPathObject, decltype(&xmlXPathFreeObject)>
xpathSearch(xmlDocPtr doc, const xmlChar *xpath){

  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;

  context = xmlXPathNewContext(doc);
  if (context == NULL) {
    fprintf(stderr, "Error in xmlXPathNewContext\n");
    return {nullptr, xmlXPathFreeObject};
  }
  result = xmlXPathEvalExpression(xpath, context);
  xmlXPathFreeContext(context);
  if (result == NULL) {
    fprintf(stderr, "Error in xmlXPathEvalExpression\n");
    return {nullptr, xmlXPathFreeObject};
  }
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    fprintf(stderr, "No result\n");
    return {nullptr, xmlXPathFreeObject};
  }
  return {result, xmlXPathFreeObject};
}

std::unique_ptr<xmlXPathObject, decltype(&xmlXPathFreeObject)>
xpathSearch(xmlDocPtr doc, const std::string& xpath) {
  return xpathSearch(doc, reinterpret_cast<const xmlChar *>(xpath.c_str()));
}

std::unique_ptr<xmlXPathObject, decltype(&xmlXPathFreeObject)>
xpathSearch(xmlNodePtr doc, const std::string& xpath) {
  return xpathSearch(reinterpret_cast<xmlDocPtr>(doc), reinterpret_cast<const xmlChar *>(xpath.c_str()));
}

void xmlErrorHandler(void *ctx, const char* msg, ...){

}

int main(int argc, char *argv[])
{

  const char* url = "https://naver.com";
  std::string response;

  // init curl and request url
  Curlpp curlpp;
  if(!curlpp.init() || !curlpp.request(url)){
    return 0;
  }

  // get response result
  response.resize(curlpp.bufferSize() + 1);
  curlpp.get(const_cast<char *>(response.data()), response.size());

//  cout << response << endl;

  // parse string
  xmlSetGenericErrorFunc(NULL, xmlErrorHandler);

  std::unique_ptr<
      std::remove_pointer_t<htmlDocPtr>,
      std::function<void(htmlDocPtr)>>
  doc = {
      htmlParseDoc(reinterpret_cast<const xmlChar *>(response.c_str()), "UTF-8"),
      [](htmlDocPtr doc){
        xmlFreeDoc(doc);
        xmlCleanupParser();
  }};

  if (doc == NULL) {
    std::cerr << "Error while parsing html\n";
    return -1;
  }

  // xpath example
  auto result = xpathSearch(doc.get(), "//div[@class='issue_area']");
  if (result) {
    auto nodeset = result->nodesetval;
    for (int i=0; i < nodeset->nodeNr; i++) {
      auto node = nodeset->nodeTab[i];


      auto news = xpathSearch(node, "//a[@href]");
      if (news) {
        auto nodeset2 = news->nodesetval;
        for(int j=0; j<nodeset2->nodeNr; j++) {
          auto node2 = nodeset2->nodeTab[j];
          printNode(node2);
        }
      }
    }
  }

  return EXIT_SUCCESS;
}