/***************************************************************************
 *   Copyright (C) 2010 by Ralf Kaestner and Luciano Spinello              *
 *   ralf.kaestner@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <signal.h>
#include <fstream>

#include <png.h>

#include "firecam.h"
#include "utils.h"

int quit = 0;

void firecam_signaled(int signal) {
  quit = 1;
}

void firecam_write_png(const FireCAMFrame& frame, const char* filename) {
  FILE* file = fopen(filename, "wb");
  png_byte colorType;
  png_byte bitDepth;

  if (frame.getColor() == FireCAMColor::rgb8) {
    bitDepth = 8;
    colorType = PNG_COLOR_TYPE_RGB;
  }
  else if (frame.getColor() == FireCAMColor::mono8) {
    bitDepth = 8;
    colorType = PNG_COLOR_TYPE_GRAY;
  }
  else
    FireCAMUtils::error("Failed to convert frame into PNG",
    "Unsupported frame color coding");

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!png)
    FireCAMUtils::error("Failed to convert frame into PNG",
    "png_create_write_struct failed");

  png_infop info = png_create_info_struct(png);
  if (!info)
    FireCAMUtils::error("Failed to convert frame into PNG",
    "png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png)))
    FireCAMUtils::error("Failed to convert frame into PNG",
    "Error during init_io");
  png_init_io(png, file);

  if (setjmp(png_jmpbuf(png)))
    FireCAMUtils::error("Failed to convert frame into PNG",
    "Error during writing header");
  png_set_IHDR(png, info, frame.getWidth(), frame.getHeight(),
    bitDepth, colorType, PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png, info);

  png_bytep rows[frame.getHeight()];
  for (int i = 0; i < frame.getHeight(); ++i)
    rows[i] = (png_byte*)&frame.getImage()[i*frame.getWidth()*
      frame.getColor().getDepth()/8];

  if (setjmp(png_jmpbuf(png)))
    FireCAMUtils::error("Failed to convert frame into PNG",
    "Error during writing bytes");
  png_write_image(png, rows);

  if (setjmp(png_jmpbuf(png)))
    FireCAMUtils::error("Failed to convert frame into PNG",
    "Error during end of write");
  png_write_end(png, NULL);

  fclose(file);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " FILENAMES" <<
      std::endl;
    return -1;
  }

  for (int i = 1; !quit && (i < argc); ++i) {
    FireCAMFrame frame;
    frame.load(argv[i]);

    std::cout << "Converting " << argv[i] << std::endl;
    std::string filename(argv[i]);
    int i = filename.find(".raw");
    if (i >= 0)
      filename.replace(i, 4,".png");
    else
      filename += ".png";

    firecam_write_png(frame, filename.c_str());
  }

  return 0;
}
