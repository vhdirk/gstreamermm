/*
 * pluginbin.h
 *
 *  Created on: 8 sie 2013
 *      Author: loganek
 */

#ifndef CUSTOMBIN_H_
#define CUSTOMBIN_H_

#include <gstreamermm.h>
#include <gstreamermm/private/bin_p.h>
#include <gstreamermm/filesrc.h>
#include <gstreamermm/filesink.h>
#include <glibmm.h>
#include <cstdio>
#include <sys/stat.h>

class PluginBin: public Gst::Bin
{
private:
    Glib::RefPtr<Gst::GhostPad> srcpad;
    Glib::Property<Glib::ustring> location;

public:
    static void base_init(BaseClassType *klass)
    {
        GstElementClass* element_klass = (GstElementClass*)klass;
        gst_element_class_set_details_simple(element_klass, "Custom test bin",
                "test/bins", "test bin", "author");
    }

    explicit PluginBin(GstBin *gobj)
    : Glib::ObjectBase(typeid (PluginBin)),
      Gst::Bin(gobj),
      location(*this, "location")
    {
        add_pad(srcpad = Gst::GhostPad::create( Gst::PadTemplate::create("src", Gst::PAD_SRC, Gst::PAD_ALWAYS,
                                Gst::Caps::create_any()), "src"));
    }

    static bool register_pluginbin(Glib::RefPtr<Gst::Plugin> plugin)
    {
        Gst::ElementFactory::register_element(plugin, "pluginbin", 10,
                Gst::register_mm_type<PluginBin>("pluginbin"));

        return true;
    }

    virtual ~PluginBin() { }

    virtual Gst::StateChangeReturn change_state_vfunc(Gst::StateChange transition)
    {
        if (transition == Gst::STATE_CHANGE_NULL_TO_READY)
        {
            Glib::RefPtr<Gst::FileSrc> source_file = Gst::FileSrc::create("source-file");
            Glib::RefPtr<Gst::Element> png_decoder = Gst::ElementFactory::create_element("pngdec");
            Glib::ustring str = "resources/input-image.png";
            source_file->property_location() = location.get_value();

            add(source_file);
            add(png_decoder);
            source_file->link(png_decoder);

            srcpad->set_target(png_decoder->get_static_pad("src"));
        }

        return Bin::change_state_vfunc(transition);
    }

};



#endif /* CUSTOMBIN_H_ */
