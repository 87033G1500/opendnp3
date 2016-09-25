package com.automatak.dnp3.codegen

case class JCacheGenerator(classes: List[ClassConfig]) {

  def headerFileName : String = "JCache.h"

  def implFileName : String = "JCache.cpp"

  def header(implicit indent: Indentation): Iterator[String] = {

    def classIncludes : Iterator[String] = classes.map(c => "#include \"JNI%s.h\"".format(c.clazz.getSimpleName)).toIterator

    def instances : Iterator[String] = classes.map(c => "static jni::%s j%s;".format(c.clazz.getSimpleName, c.clazz.getSimpleName)).toIterator

    commented(LicenseHeader()) ++ space ++
      includeGuards("OPENDNP3_JNITYPES") {
        "#include \"openpal/util/Uncopyable.h\"".iter ++ space
        classIncludes ++ space ++
        namespace("jcache") {
          "bool init(JNIEnv* env);".iter ++ space ++
          instances
        }
      }
  }

  def impl(implicit indent: Indentation): Iterator[String] = {

    def instances : Iterator[String] = {
      classes.flatMap {
        c => {
          "success = j%s.init(env);".format(c.clazz.getSimpleName).iter ++
          "if(!success) return false;".iter
        }
      }.toIterator
    }

    commented(LicenseHeader()) ++ space ++
    "#include \"JCache.h\"".iter ++ space ++
    namespace("jcache") {
      "bool init(JNIEnv* env)".iter ++ bracket {
        "auto success = true;".iter ++ space ++
        instances ++
        "return true;".iter
      }
    }


  }

}
