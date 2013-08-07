/*
 * test-foo.h
 *
 *  Created on: Aug 1, 2013
 *      Author: m.kolny
 */

#ifndef TEST_FOO_H_
#define TEST_FOO_H_

#include <gstreamermm.h>
#include <gstreamermm/private/element_p.h>
#include <assert.h>

using namespace Gst;

class Foo : public Element
{
    Glib::RefPtr<Pad> sinkpad;
    Glib::RefPtr<Pad> srcpad;
    Glib::Property<Glib::ustring> sample_property;

public:
    static void base_init(BaseClassType *klass)
    {
        /* This is another hack.
         * For now it uses pure C functions, which should be wrapped then.
         */
        gst_element_class_set_details_simple(klass, "foo_longname",
                "foo_classification", "foo_detail_description", "foo_detail_author");

        gst_element_class_add_pad_template(klass,
                Gst::PadTemplate::create("sink", Gst::PAD_SINK, Gst::PAD_ALWAYS,
                        Gst::Caps::create_any())->gobj());
        gst_element_class_add_pad_template(klass,
                Gst::PadTemplate::create("src", Gst::PAD_SRC, Gst::PAD_ALWAYS,
                        Gst::Caps::create_any())->gobj());
    }

    FlowReturn chain(const Glib::RefPtr<Gst::Pad> &pad, Glib::RefPtr<Gst::Buffer> &buf)
    {
        buf = buf->create_writable();
        assert(buf->gobj()->mini_object.refcount==1);
        Glib::RefPtr<Gst::MapInfo> mapinfo(new Gst::MapInfo());
        buf->map(mapinfo, Gst::MAP_WRITE);
        std::sort(mapinfo->get_data(), mapinfo->get_data() + mapinfo->get_size());
        buf->unmap(mapinfo);
        assert(buf->gobj()->mini_object.refcount==1);
        return srcpad->push(buf);
    }

    explicit Foo(GstElement *gobj)
        : Glib::ObjectBase(typeid (Foo)), // type must be registered before use
          Gst::Element(gobj),
          sample_property(*this, "sample_property", "def_val")

    {
        add_pad(sinkpad = Gst::Pad::create(get_pad_template("sink"), "sink"));
        add_pad(srcpad = Gst::Pad::create(get_pad_template("src"), "src"));
        sinkpad->set_chain_function(sigc::mem_fun(*this, &Foo::chain));
    }

    static bool register_foo(Glib::RefPtr<Gst::Plugin> plugin)
    {
        ElementFactory::register_element(plugin, "foomm", 10, register_mm_type<Foo>("foomm"));

        return true;
    }
};

#endif /* TEST_FOO_H_ */
