

import cv2
import math
import numpy as np
import torch

class Blend:
    def __init__(self) -> None:
        self.width = 256
        self.height = 256
        self._radius = 0

    @property
    def radius(self):
        return self._radius

    def create_mask(self, radius):
        self._radius = radius
        image = np.zeros((self.height, self.width), dtype=np.uint8)
        center_x, center_y = self.width // 2, self.height // 2
        A = 255
        for y in range(self.height):
            for x in range(self.width):
                distance = math.sqrt((x - center_x) ** 2 + (y - center_y) ** 2)
                if distance > radius:
                    # 计算当前像素距离圆心的距离与半径的比例
                    ratio = distance / radius
                    r = 3 - 2 * ratio
                    coff = np.power(5, r)
                    index = 2 / (1 + np.exp(pow(radius, -0.7621) * (distance - radius)))
                    # 根据比例计算像素值
                    pixel_value = int(A * index)
                    # 填充像素
                    image[y, x] = pixel_value
        for y in range(self.height):
            for x in range(self.width):
                distance = math.sqrt((x - center_x) ** 2 + (y - center_y) ** 2)
                if distance - radius < 0.51:
                    pixel_value = 255
                    # 填充像素
                    image[y, x] = pixel_value

        return image

    def blend(self, mask_radius, image):
        mask = self.create_mask(mask_radius)
        # 将图像转换为PyTorch张量，并归一化到0到1之间
        image_tensor = torch.from_numpy(image.transpose(2, 0, 1)).float() / 255.0
        # 将掩膜转换为PyTorch张量，并归一化到0到1之间
        mask_tensor = torch.from_numpy(mask).float() / 255.0
        # 计算掩膜的反掩膜
        mask_inv = 1 - mask_tensor
        # 使用掩膜和反掩膜来融合图像
        masked_image_tensor = image_tensor * mask_inv.unsqueeze(0) + mask_tensor.unsqueeze(0)
        # 将融合后的图像转换为NumPy数组，并重新排列维度顺序
        masked_image_np = masked_image_tensor.numpy().transpose(1, 2, 0)
        # 将像素值限制在0到1之间，并将浮点数乘以255并转换为无符号整数
        masked_image_np = np.clip(masked_image_np, 0, 1) * 255
        masked_image_np = masked_image_np.astype(np.uint8)
        # 返回融合后的图像
        return masked_image_np

if __name__ == '__main__':

    image_path = f"image/airplane{0}.jpg"
    image = cv2.imread(image_path)
    app = Blend()
    image_ret = app.blend(31, image)
    cv2.imwrite(f"./mask_{app.radius}.png", image_ret)

