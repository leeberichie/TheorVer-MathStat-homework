import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Polygon, Rectangle
import matplotlib.patches as mpatches

# Параметры задачи
T = 80
t1 = 15
t2 = 20

fig, axes = plt.subplots(2, 3, figsize=(18, 12))
fig.suptitle('ДЗ1 Задача 1', 
             fontsize=16, fontweight='bold')

# Функция для настройки осей
def setup_axis(ax, title):
    ax.set_xlim(0, T)
    ax.set_ylim(0, T)
    ax.set_xlabel('x (время поступления 1-й заявки)', fontsize=11)
    ax.set_ylabel('y (время поступления 2-й заявки)', fontsize=11)
    ax.set_title(title, fontsize=12, fontweight='bold')
    ax.grid(True, alpha=0.3)
    ax.set_aspect('equal')

# 1) Вероятность обслуживания обеих заявок
ax1 = axes[0, 0]
setup_axis(ax1, '1) Обслуживание обеих заявок')

# Область всех возможных исходов
ax1.add_patch(Rectangle((0, 0), T, T, fill=False, edgecolor='black', linewidth=2))

# Условия обслуживания обеих заявок:
# Если x < y: y > x + t1 и y + t2 <= T
# Если x > y: x > y + t2 и x + t1 <= T

# Область: x < y, y > x + t1, y <= T - t2
x_vals1 = np.linspace(0, T - t1, 1000)
y_lower1 = x_vals1 + t1
y_upper1 = np.full_like(x_vals1, T - t2)
mask1 = y_upper1 >= y_lower1

vertices1 = []
for i in range(len(x_vals1)):
    if mask1[i]:
        vertices1.append([x_vals1[i], y_lower1[i]])
for i in range(len(x_vals1)-1, -1, -1):
    if mask1[i]:
        vertices1.append([x_vals1[i], y_upper1[i]])

if vertices1:
    poly1 = Polygon(vertices1, alpha=0.5, facecolor='blue', edgecolor='blue', linewidth=2)
    ax1.add_patch(poly1)

# Область: x > y, x > y + t2, x <= T - t1
y_vals2 = np.linspace(0, T - t2, 1000)
x_lower2 = y_vals2 + t2
x_upper2 = np.full_like(y_vals2, T - t1)
mask2 = x_upper2 >= x_lower2

vertices2 = []
for i in range(len(y_vals2)):
    if mask2[i]:
        vertices2.append([x_lower2[i], y_vals2[i]])
for i in range(len(y_vals2)-1, -1, -1):
    if mask2[i]:
        vertices2.append([x_upper2[i], y_vals2[i]])

if vertices2:
    poly2 = Polygon(vertices2, alpha=0.5, facecolor='blue', edgecolor='blue', linewidth=2)
    ax1.add_patch(poly2)

# Границы
ax1.plot([0, T-t1], [t1, T], 'r--', linewidth=1.5, label='y = x + t1')
ax1.plot([t2, T], [0, T-t2], 'g--', linewidth=1.5, label='x = y + t2')
ax1.axhline(T-t2, color='orange', linestyle='--', linewidth=1.5, label=f'y = {T-t2}')
ax1.axvline(T-t1, color='purple', linestyle='--', linewidth=1.5, label=f'x = {T-t1}')
ax1.legend(fontsize=9)

# 2) Вероятность обслуживания только одной заявки
ax2 = axes[0, 1]
setup_axis(ax2, '2) Обслуживание только одной заявки')

ax2.add_patch(Rectangle((0, 0), T, T, fill=False, edgecolor='black', linewidth=2))

# Только первая: x + t1 <= T, но не обе
# Область 1: x < y < x + t1, x + t1 <= T
x_vals = np.linspace(0, T-t1, 1000)
y_lower = x_vals
y_upper = x_vals + t1
vertices_only1a = [[x_vals[i], y_lower[i]] for i in range(len(x_vals))]
vertices_only1a += [[x_vals[i], y_upper[i]] for i in range(len(x_vals)-1, -1, -1)]
poly = Polygon(vertices_only1a, alpha=0.4, facecolor='cyan', edgecolor='cyan', linewidth=1.5)
ax2.add_patch(poly)

# Область 2: x + t1 <= T, y > T - t2
x_vals = np.linspace(0, T-t1, 1000)
vertices_only1b = [[x_vals[0], T-t2], [x_vals[-1], T-t2], [x_vals[-1], T], [x_vals[0], T]]
poly = Polygon(vertices_only1b, alpha=0.4, facecolor='cyan', edgecolor='cyan', linewidth=1.5)
ax2.add_patch(poly)

# Только вторая: y + t2 <= T, но не обе
# Область 3: y < x < y + t2, y + t2 <= T
y_vals = np.linspace(0, T-t2, 1000)
x_lower = y_vals
x_upper = y_vals + t2
vertices_only2a = [[x_lower[i], y_vals[i]] for i in range(len(y_vals))]
vertices_only2a += [[x_upper[i], y_vals[i]] for i in range(len(y_vals)-1, -1, -1)]
poly = Polygon(vertices_only2a, alpha=0.4, facecolor='magenta', edgecolor='magenta', linewidth=1.5)
ax2.add_patch(poly)

# Область 4: y + t2 <= T, x > T - t1
y_vals = np.linspace(0, T-t2, 1000)
vertices_only2b = [[T-t1, y_vals[0]], [T, y_vals[0]], [T, y_vals[-1]], [T-t1, y_vals[-1]]]
poly = Polygon(vertices_only2b, alpha=0.4, facecolor='magenta', edgecolor='magenta', linewidth=1.5)
ax2.add_patch(poly)

ax2.plot([0, T], [0, T], 'k--', linewidth=1, alpha=0.5)
ax2.legend([mpatches.Patch(color='cyan', alpha=0.4), mpatches.Patch(color='magenta', alpha=0.4)],
          ['Только 1-я', 'Только 2-я'], fontsize=9)

# 3) P(A1|A2) - первая обслужена при условии, что вторая обслужена
ax3 = axes[0, 2]
setup_axis(ax3, '3) P(обслужена 1-я | обслужена 2-я)')

ax3.add_patch(Rectangle((0, 0), T, T, fill=False, edgecolor='black', linewidth=2))

# Условие A2: y + t2 <= T
ax3.axhline(T-t2, color='red', linestyle='--', linewidth=2, label=f'y = {T-t2} (условие A2)')
ax3.fill_between([0, T], 0, T-t2, alpha=0.2, color='yellow', label='A2 (2-я обслужена)')

# A1 ∩ A2: обе обслужены (из пункта 1)
if vertices1:
    poly1 = Polygon(vertices1, alpha=0.6, facecolor='green', edgecolor='green', linewidth=2, label='A1 ∩ A2')
    ax3.add_patch(poly1)
if vertices2:
    poly2 = Polygon(vertices2, alpha=0.6, facecolor='green', edgecolor='green', linewidth=2)
    ax3.add_patch(poly2)

ax3.legend(fontsize=9)

# 4) P(A2|A1) - вторая обслужена при условии, что первая обслужена
ax4 = axes[1, 0]
setup_axis(ax4, '4) P(обслужена 2-я | обслужена 1-я)')

ax4.add_patch(Rectangle((0, 0), T, T, fill=False, edgecolor='black', linewidth=2))

# Условие A1: x + t1 <= T
ax4.axvline(T-t1, color='red', linestyle='--', linewidth=2, label=f'x = {T-t1} (условие A1)')
ax4.fill_betweenx([0, T], 0, T-t1, alpha=0.2, color='yellow', label='A1 (1-я обслужена)')

# A1 ∩ A2: обе обслужены
if vertices1:
    poly1 = Polygon(vertices1, alpha=0.6, facecolor='green', edgecolor='green', linewidth=2, label='A1 ∩ A2')
    ax4.add_patch(poly1)
if vertices2:
    poly2 = Polygon(vertices2, alpha=0.6, facecolor='green', edgecolor='green', linewidth=2)
    ax4.add_patch(poly2)

ax4.legend(fontsize=9)

# 5) P(x < y | обе обслужены) - первая раньше второй при условии обслуживания обеих
ax5 = axes[1, 1]
setup_axis(ax5, '5) P(1-я раньше 2-й | обе обслужены)')

ax5.add_patch(Rectangle((0, 0), T, T, fill=False, edgecolor='black', linewidth=2))

# Обе обслужены (полупрозрачно)
if vertices1:
    poly1 = Polygon(vertices1, alpha=0.3, facecolor='gray', edgecolor='blue', linewidth=1.5)
    ax5.add_patch(poly1)
if vertices2:
    poly2 = Polygon(vertices2, alpha=0.3, facecolor='gray', edgecolor='blue', linewidth=1.5)
    ax5.add_patch(poly2)

# x < y и обе обслужены (только верхняя область)
if vertices1:
    poly1 = Polygon(vertices1, alpha=0.7, facecolor='red', edgecolor='red', linewidth=2, label='x < y, обе обслужены')
    ax5.add_patch(poly1)

ax5.plot([0, T], [0, T], 'k--', linewidth=2, label='x = y')
ax5.legend(fontsize=9)

# Убираем последний пустой subplot
axes[1, 2].axis('off')
axes[1, 2].text(0.5, 0.5, 'Расчет вероятностей:\n\n' +
                f'S = {T}² = {T**2}\n\n' +
                '1) P(обе) = S(обе)/S\n' +
                '2) P(одна) = S(одна)/S\n' +
                '3) P(A1|A2) = S(обе)/S(A2)\n' +
                '4) P(A2|A1) = S(обе)/S(A1)\n' +
                '5) P(x<y|обе) = S(x<y,обе)/S(обе)',
                ha='center', va='center', fontsize=12, 
                bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))

plt.tight_layout()
plt.show()