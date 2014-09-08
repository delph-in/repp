/*
 * Get the necessary settings from cheap_settings to create ReppTokenizer
 */

#include <string>
#include "repp_from_pet.h"
#include "errors.h"

using namespace std;

tReppTokenizer *createReppTokenizer(settings *cheap_settings)
{
  tdlOptions *repp_opts = new tdlOptions();
  string rpp_basedir = cheap_settings->base() + "rpp/";
  repp_opts->set("repp-directory", rpp_basedir);
  repp_opts->set("repp-tokenizer", cheap_settings->value("repp-tokenizer"));
  struct setting *foo;
  if ((foo = cheap_settings->lookup("repp-modules")) != NULL) {
    for (int i = 0; i < foo->n; ++i) {
      repp_opts->add("repp-modules", foo->values[i]);
    }
  }
  if ((foo = cheap_settings->lookup("repp-calls")) != NULL) {
    for (int i = 0; i < foo->n; ++i) {
      repp_opts->add("repp-calls", foo->values[i]);
    }
  }
  tReppTokenizer *tok = new tReppTokenizer(repp_opts);
  return tok;
}
