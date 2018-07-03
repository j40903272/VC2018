import cv2
import argparse
from skimage.io import imread, imsave

from inpainter import Inpainter


def main():
    args = parse_args()

    image = imread(args.input_image)
    #image = cv2.imread(args.input_image)
    #image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    mask = imread(args.mask, as_grey=True)
    print ('start inpaint', image.shape, mask.shape)
    output_image = Inpainter(
        image,
        mask,
        patch_size=args.patch_size,
        plot_progress=args.plot_progress
    ).inpaint()
    print ('finish!', 'save to', args.output)
    imsave(args.output, output_image, quality=100)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-ps',
        '--patch-size',
        help='the size of the patches',
        type=int,
        default=9
    )
    parser.add_argument(
        '-o',
        '--output',
        help='the file path to save the output image',
        default='output.jpg'
    )
    parser.add_argument(
        '--plot-progress',
        help='plot each generated image',
        action='store_true',
        default=False
    )
    parser.add_argument(
        'input_image',
        help='the image containing objects to be removed'
    )
    parser.add_argument(
        'mask',
        help='the mask of the region to be removed'
    )
    return parser.parse_args()


if __name__ == '__main__':
    main()
