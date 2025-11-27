import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Polygon, Rectangle
import matplotlib.patches as mpatches

# Параметры задачи (нормализуем L = 1)
L = 1.0
k1 = 0.9
k2 = 0.75
k = 0.15

fig, axes = plt.subplots(2, 3, figsize=(18, 12))
fig.suptitle('ДЗ1 Задача 2', 
             fontsize=16, fontweight='bold')

# Функция для настройки осей
def setup_axis(ax, title):
    ax.set_xlim(0, L)
    ax.set_ylim(0, L)
    ax.set_xlabel('x = AM (положение точки M)', fontsize=11)
    ax.set_ylabel('y = AN (положение точки N)', fontsize=11)
    ax.set_title(title, fontsize=12, fontweight='bold')
    ax.grid(True, alpha=0.3)
    ax.set_aspect('equal')

# Область допустимых значений: x < k1*L и (L - y) < k2*L => y > L - k2*L
x_max = k1 * L
y_min = L - k2 * L

# 1) P(AM < AN) => P(x < y)
ax1 = axes[0, 0]
setup_axis(ax1, '1) AM < AN (x < y)')

# Прямоугольник допустимой области
ax1.add_patch(Rectangle((0, y_min), x_max, L-y_min, 
                         fill=True, facecolor='lightgray', 
                         edgecolor='black', linewidth=2, alpha=0.3, label='Допустимая область'))

# Область x < y в допустимой области
x_vals = np.linspace(0, x_max, 1000)
y_lower = np.maximum(x_vals, y_min)
y_upper = np.full_like(x_vals, L)

vertices = []
for i in range(len(x_vals)):
    if y_upper[i] >= y_lower[i]:
        vertices.append([x_vals[i], y_lower[i]])
for i in range(len(x_vals)-1, -1, -1):
    if y_upper[i] >= y_lower[i]:
        vertices.append([x_vals[i], y_upper[i]])

if vertices:
    poly = Polygon(vertices, alpha=0.6, facecolor='blue', edgecolor='blue', linewidth=2, label='x < y')
    ax1.add_patch(poly)

ax1.plot([0, L], [0, L], 'r--', linewidth=2, label='x = y')
ax1.axvline(x_max, color='green', linestyle='--', linewidth=2, label=f'x = {k1}L')
ax1.axhline(y_min, color='orange', linestyle='--', linewidth=2, label=f'y = {y_min}L')
ax1.legend(fontsize=9, loc='upper left')

# 2) P(MN < k*L)
# Части: AM = x, MN = |y - x|, NB = L - y (если x < y) или AM = x, NM = x - y, NB = L - y (если x > y)
# |y - x| < k*L => -k*L < y - x < k*L => x - k*L < y < x + k*L
ax2 = axes[0, 1]
setup_axis(ax2, f'2) MN < {k}L')

ax2.add_patch(Rectangle((0, y_min), x_max, L-y_min, 
                         fill=True, facecolor='lightgray', 
                         edgecolor='black', linewidth=2, alpha=0.3))

# Полоса между y = x - k*L и y = x + k*L
x_vals = np.linspace(0, x_max, 1000)
y_lower_band = np.maximum(x_vals - k*L, y_min)
y_upper_band = np.minimum(x_vals + k*L, L)

vertices = []
for i in range(len(x_vals)):
    if y_upper_band[i] >= y_lower_band[i]:
        vertices.append([x_vals[i], y_lower_band[i]])
for i in range(len(x_vals)-1, -1, -1):
    if y_upper_band[i] >= y_lower_band[i]:
        vertices.append([x_vals[i], y_upper_band[i]])

if vertices:
    poly = Polygon(vertices, alpha=0.6, facecolor='cyan', edgecolor='cyan', linewidth=2, label=f'|y-x| < {k}L')
    ax2.add_patch(poly)

ax2.plot([0, L], [k*L, L+k*L], 'r--', linewidth=1.5, label=f'y = x + {k}L')
ax2.plot([k*L, L], [0, L-k*L], 'g--', linewidth=1.5, label=f'y = x - {k}L')
ax2.axvline(x_max, color='purple', linestyle='--', linewidth=1.5)
ax2.axhline(y_min, color='orange', linestyle='--', linewidth=1.5)
ax2.legend(fontsize=9, loc='upper left')

# 3) Неравенство треугольника
# Части отрезка (упорядоченные): x, y-x, L-y (если x < y) или x-y, y, L-x (если x > y и y < x)
# Для треугольника: сумма двух меньших сторон > большей стороны
ax3 = axes[0, 2]
setup_axis(ax3, '3) Можно составить треугольник')

ax3.add_patch(Rectangle((0, y_min), x_max, L-y_min, 
                         fill=True, facecolor='lightgray', 
                         edgecolor='black', linewidth=2, alpha=0.3))

# Рассматриваем случаи
# Случай 1: x < y (части: x, y-x, L-y)
# Неравенство треугольника: x + (y-x) > L-y => y > L/2
#                           x + (L-y) > y-x => 2x + L - y > y => 2x > 2y - L => x > y - L/2
#                           (y-x) + (L-y) > x => L - x > x => x < L/2

# Случай 2: x > y (части: y, x-y, L-x)
# Неравенство треугольника: y + (x-y) > L-x => x > L/2
#                           y + (L-x) > x-y => 2y + L - x > x => 2y > 2x - L => y > x - L/2
#                           (x-y) + (L-x) > y => L - y > y => y < L/2

n_points = 2000
x_grid = np.linspace(0, x_max, n_points)
y_grid = np.linspace(y_min, L, n_points)
X, Y = np.meshgrid(x_grid, y_grid)

# Условие для треугольника
triangle_condition = np.zeros_like(X, dtype=bool)

# Случай x < y
mask1 = X < Y
cond1 = (Y > L/2) & (X > Y - L/2) & (X < L/2)
triangle_condition = triangle_condition | (mask1 & cond1)

# Случай x > y
mask2 = X > Y
cond2 = (X > L/2) & (Y > X - L/2) & (Y < L/2)
triangle_condition = triangle_condition | (mask2 & cond2)

ax3.contourf(X, Y, triangle_condition.astype(int), levels=[0.5, 1.5], colors=['green'], alpha=0.6)

# Границы
ax3.plot([0, L], [L/2, L/2], 'r--', linewidth=1.5, label='y = L/2')
ax3.plot([L/2, L/2], [0, L], 'b--', linewidth=1.5, label='x = L/2')
ax3.plot([0, L], [0, L], 'k--', linewidth=1, alpha=0.5, label='x = y')
ax3.plot([L/2, L], [0, L/2], 'm--', linewidth=1.5, label='y = x - L/2')
ax3.plot([0, L/2], [L/2, L], 'c--', linewidth=1.5, label='x = y - L/2')
ax3.axvline(x_max, color='purple', linestyle='--', linewidth=1.5)
ax3.axhline(y_min, color='orange', linestyle='--', linewidth=1.5)
ax3.legend(fontsize=8, loc='upper left')

# 4) Длина самой короткой части ≤ 0.5 * длина самой длинной части
ax4 = axes[1, 0]
setup_axis(ax4, '4) min(части) ≤ 0.5 × max(части)')

ax4.add_patch(Rectangle((0, y_min), x_max, L-y_min, 
                         fill=True, facecolor='lightgray', 
                         edgecolor='black', linewidth=2, alpha=0.3))

# Условие: 2*min ≤ max
condition4 = np.zeros_like(X, dtype=bool)

for i in range(X.shape[0]):
    for j in range(X.shape[1]):
        x_val = X[i, j]
        y_val = Y[i, j]
        
        if x_val < y_val:
            parts = [x_val, y_val - x_val, L - y_val]
        else:
            parts = [y_val, x_val - y_val, L - x_val]
        
        parts = sorted(parts)
        if 2 * parts[0] <= parts[2]:
            condition4[i, j] = True

ax4.contourf(X, Y, condition4.astype(int), levels=[0.5, 1.5], colors=['orange'], alpha=0.6)

ax4.axvline(x_max, color='purple', linestyle='--', linewidth=1.5)
ax4.axhline(y_min, color='orange', linestyle='--', linewidth=1.5)
ax4.plot([0, L], [0, L], 'k--', linewidth=1, alpha=0.5)
ax4.legend([mpatches.Patch(color='orange', alpha=0.6)], 
           ['2×min ≤ max'], fontsize=9, loc='upper left')

# 5) S(прямоугольник min×max) < S(квадрат средней стороны)
# min * max < средняя²
ax5 = axes[1, 1]
setup_axis(ax5, '5) min × max < средняя²')

ax5.add_patch(Rectangle((0, y_min), x_max, L-y_min, 
                         fill=True, facecolor='lightgray', 
                         edgecolor='black', linewidth=2, alpha=0.3))

condition5 = np.zeros_like(X, dtype=bool)

for i in range(X.shape[0]):
    for j in range(X.shape[1]):
        x_val = X[i, j]
        y_val = Y[i, j]
        
        if x_val < y_val:
            parts = [x_val, y_val - x_val, L - y_val]
        else:
            parts = [y_val, x_val - y_val, L - x_val]
        
        parts = sorted(parts)
        if parts[0] * parts[2] < parts[1]**2:
            condition5[i, j] = True

ax5.contourf(X, Y, condition5.astype(int), levels=[0.5, 1.5], colors=['red'], alpha=0.6)

ax5.axvline(x_max, color='purple', linestyle='--', linewidth=1.5)
ax5.axhline(y_min, color='orange', linestyle='--', linewidth=1.5)
ax5.plot([0, L], [0, L], 'k--', linewidth=1, alpha=0.5)
ax5.legend([mpatches.Patch(color='red', alpha=0.6)], 
           ['min×max < средняя²'], fontsize=9, loc='upper left')

# Информационная панель
ax6 = axes[1, 2]
ax6.axis('off')
ax6.text(0.5, 0.5, 
         'Параметры задачи:\n\n' +
         f'L = {L}\n' +
         f'k₁ = {k1} → AM < {k1}L\n' +
         f'k₂ = {k2} → NB < {k2}L\n' +
         f'k = {k} (для пункта 2)\n\n' +
         'Допустимая область:\n' +
         f'0 ≤ x ≤ {k1}L = {x_max}\n' +
         f'{y_min}L ≤ y ≤ L\n\n' +
         'Части отрезка:\n' +
         '• Если x < y: {x, y-x, L-y}\n' +
         '• Если x > y: {y, x-y, L-x}\n\n' +
         'Вероятность:\n' +
         'P = S(закрашенная)/S(допустимая)',
         ha='center', va='center', fontsize=11, 
         bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5),
         family='monospace')

plt.tight_layout()
plt.show()

# Расчет вероятностей
S_total = x_max * (L - y_min)
print(f"\nПлощадь допустимой области: {S_total:.4f}")
print(f"Параметры: k1={k1}, k2={k2}, k={k}")
print(f"Границы: x ∈ [0, {x_max}], y ∈ [{y_min}, {L}]")