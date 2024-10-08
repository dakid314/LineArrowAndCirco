#include "SDK_Drawer.hpp"
#include <sstream>
#include <stdexcept>
#include <cmath>

namespace SDK_Draw {

double annotate_font_size = 20;

std::function<bool(const double, const double)> UpandDown = [](const double s, const double e) -> bool { return s < e ? true : false; };

void annotate(cr::cairo_surface_t* surface, const double& Present, const SDK_Core::SIZE2D& pagesize, const char* context, const double linewidth, const bool upAndDpwn)
{
    cr::cairo_t* pan = cr::cairo_create(surface);
    cr::cairo_set_line_cap(pan, cr::CAIRO_LINE_CAP_ROUND);
    cr::cairo_set_line_width(pan, annotate_line_w);
    cr::cairo_set_source_rgb(pan, annotate_line_color.R, annotate_line_color.G, annotate_line_color.B);

    cr::cairo_select_font_face(pan, "Sans", cr::CAIRO_FONT_SLANT_NORMAL, cr::CAIRO_FONT_WEIGHT_BOLD);
    cr::cairo_set_font_size(pan, annotate_font_size);

    cr::cairo_text_extents_t extents;
    cr::cairo_text_extents(pan, context, &extents);

    SDK_Core::POINT2D vector;
    // Right
    if (upAndDpwn == false) { // Left
        vector = { gap + (Present) * (pagesize.W - 2 * gap) - extents.width / 2, annotate_font_line_gap + 1.5 * linewidth + arrow_line_gap + 2 * extents.height };
    } else {
        vector = { gap + (Present) * (pagesize.W - 2 * gap) - extents.width / 2, 0.0 - annotate_font_line_gap - 0.5 * linewidth - extents.height };
    }

    cr::cairo_set_source_rgb(pan, annotate_text_color.R, annotate_text_color.G, annotate_text_color.B);
    cr::cairo_move_to(pan, vector.X, pagesize.H / 2 + vector.Y);
    cr::cairo_show_text(pan, context);
    cr::cairo_destroy(pan);
}

void arrow(cr::cairo_surface_t* surface, const double& startPresent, const double& endPresent, const SDK_Core::RGB& rgb, const SDK_Core::SIZE2D& pagesize, const double& ArrowWirth)
{
    cr::cairo_t* pan = cr::cairo_create(surface);
    // Right
    int un = 1, upline = 0;
    if (endPresent < startPresent) {
        // Left
        un = -1;
        upline = 1;
    }

    cr::cairo_set_line_cap(pan, cr::CAIRO_LINE_CAP_ROUND);
    SDK_Core::POINT2D point = { gap + (startPresent) * (pagesize.W - 2 * gap), pagesize.H / 2 + upline * (arrow_line_gap + ArrowWirth) + ArrowWirth / 2 };
    cr::cairo_move_to(pan, point.X, point.Y);

    if (un * (endPresent - startPresent) * (pagesize.W - 2 * gap) <= (ArrowWirth / 2)) {
        point.X += (endPresent - startPresent) * (pagesize.W - 2 * gap);
        point.Y -= ArrowWirth / 2;
        cr::cairo_line_to(pan, point.X, point.Y);
        point.X -= (endPresent - startPresent) * (pagesize.W - 2 * gap);
        point.Y -= ArrowWirth / 2;
        cr::cairo_line_to(pan, point.X, point.Y);
    } else {
        point.X = (endPresent) * (pagesize.W - 2 * gap) - un * (ArrowWirth / 2) + gap;
        cr::cairo_line_to(pan, point.X, point.Y);
        point.X += un * (ArrowWirth / 2);
        point.Y -= ArrowWirth / 2;
        cr::cairo_line_to(pan, point.X, point.Y);
        point.X -= un * (ArrowWirth / 2);
        point.Y -= ArrowWirth / 2;
        cr::cairo_line_to(pan, point.X, point.Y);
        point.X = gap + (startPresent) * (pagesize.W - 2 * gap);
        cr::cairo_line_to(pan, point.X, point.Y);
    }
    cr::cairo_close_path(pan);

    cr::cairo_set_source_rgb(pan, rgb.R, rgb.G, rgb.B);
    cr::cairo_fill_preserve(pan);
    cr::cairo_set_line_width(pan, 0.5);
    cr::cairo_stroke(pan);
    cr::cairo_destroy(pan);
}

void make_line(cr::cairo_surface_t* surface, const SDK_Core::RGB& line_color)
{
    cr::cairo_t* pan = cr::cairo_create(surface);
    cr::cairo_set_line_cap(pan, cr::CAIRO_LINE_CAP_ROUND);
    cr::cairo_set_line_width(pan, W_line);
    cr::cairo_set_source_rgb(pan, line_color.R, line_color.G, line_color.B);
    cr::cairo_move_to(pan, gap, PageH / 2);
    cr::cairo_line_to(pan, PageW - gap, PageH / 2);
    cr::cairo_stroke(pan);
    cr::cairo_destroy(pan);
}
void make_circo(cr::cairo_surface_t* surface)
{
    cr::cairo_t* pan = cr::cairo_create(surface);
    cr::cairo_move_to(pan, center_x + radius, center_y);
    cr::cairo_arc(pan, center_x, center_y, radius, 0, 2 * M_PI);
    cr::cairo_set_source_rgb(pan, 0.584, 0.584, 0.620);
    cr::cairo_set_line_width(pan, 10);
    cr::cairo_stroke_preserve(pan);
    cr::cairo_destroy(pan);
}

void circo_arrow(cr::cairo_surface_t* surface, const double& startPresent, const double& endPresent, const SDK_Core::RGB& rgb)
{
    double new_startPresent ;
    double new_endtPresent  ;
    if (startPresent > endPresent)
    {
        new_startPresent = endPresent;
        new_endtPresent = startPresent;
    }
    else
    {
        new_startPresent = startPresent;
        new_endtPresent = endPresent;

    }
    

    cr::cairo_t* pan = cr::cairo_create(surface);
    cr::cairo_move_to(pan, center_x + radius * cos(new_startPresent * 2 * M_PI - M_PI / 2), center_y + radius * sin(new_startPresent * 2 * M_PI - M_PI / 2)) ;
    cr::cairo_arc(pan, center_x, center_y, radius, new_startPresent * 2 * M_PI- M_PI / 2 , new_endtPresent * 2 * M_PI- M_PI / 2);
    cr::cairo_set_source_rgb(pan, rgb.R, rgb.G, rgb.B );
    cr::cairo_set_line_width(pan, 10);
    cr::cairo_stroke_preserve(pan);
    cr::cairo_destroy(pan);
}


void circo_annotate(cr::cairo_surface_t* surface, const double& Present, const char* context)
{
    cr::cairo_t* pan = cr::cairo_create(surface);
    cr::cairo_new_sub_path(pan);
    cr::cairo_select_font_face(pan, "Sans", cr::CAIRO_FONT_SLANT_NORMAL, cr::CAIRO_FONT_WEIGHT_BOLD);
    cr::cairo_set_font_size(pan, 10);
    cr::cairo_set_source_rgb(pan, 0, 0, 0);
    cr::cairo_move_to(pan, center_x + radius * cos(Present *2* M_PI- M_PI / 2), center_y + radius * sin(Present *2* M_PI- M_PI / 2));
    cr::cairo_show_text(pan, context);
    cr::cairo_destroy(pan);
}
}