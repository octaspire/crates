/*
  Crates ‐ extensible 3D puzzle game.
  Copyright (C) 2008‐2010  Octaspire (www.octaspire.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;


public class MakeFont {
  public static void main(String[] args) throws IOException {
    final int iw = 256;
    final int ih = 256;
    BufferedImage off_Image = new BufferedImage(iw, ih, BufferedImage.TYPE_INT_RGB);
    Graphics2D g2 = off_Image.createGraphics();
    g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, (RenderingHints.VALUE_TEXT_ANTIALIAS_ON));
    Font plainFont = new Font("Monospaced", Font.PLAIN, 14);
    Font boldFont  = new Font("Monospaced", Font.BOLD,  14);
    g2.setFont(plainFont);

    FontMetrics metrics = g2.getFontMetrics(plainFont);
    int hgt = metrics.getHeight();
    int adv;
    String c;
    int hx, hy;
    final int w = 16;
    final int h = 16;
    int x = 0;
    int y = h-3;
    for (int i = 32; i < (128+16); i++) {
      c = ""+(char)i;
      adv = metrics.stringWidth(c);
      hx = (adv/2);
      hy = h - (hgt/2);
      /*System.out.println(adv + " " + hgt);*/
      g2.drawString(c, x+hx, y);
      x += w;
      if (x >= (iw-(w/2))) {
        x = 0;
        y += h;
        assert(y < ih);
      }
    }

    /*y += h;*/
    x = 0;
    g2.setFont(boldFont);
    metrics = g2.getFontMetrics(boldFont);
    hgt = metrics.getHeight();
    for (int i = 32; i < (128+16); i++) {
      c = ""+(char)i;
      adv = metrics.stringWidth(c);
      hx = (adv/2);
      hy = h - (hgt/2);
      /*System.out.println(adv + " " + hgt);*/
      g2.drawString(c, x+hx, y);
      x += w;
      if (x >= (iw-(w/2))) {
        x = 0;
        y += h;
        assert(y < ih);
      }
    }

    File outputfile = new File("font.png");
    ImageIO.write(off_Image, "png", outputfile);
  }

}
